#include "global.h"
#include "dsp.h"
#include <stdint.h>
#include "global.h"
#include "main.h"
#include "arm_math.h"
#include "fft.h"
#include "agc.h"
#include <stdbool.h>


enum dspRingH {HALF_LOWER, HALF_UPPER};
volatile bool dspRingHalf = HALF_LOWER;
volatile bool dspProcDone = 0;

uint16_t  inBuf[ADC_BUFFER_LEN] = {0};
uint16_t  outBuf[ADC_BUFFER_LEN/2] = {0};

#define NUM_TAPS  			32

const q31_t firCoeffs32[NUM_TAPS]= {
	 51418007,
	55039918,
	38677951,
	3953513,
	-42059779,
	-87891547,
	-119932313,
	-125439197,
	-95623309,
	-28092393,
	71964461,
	192197614,
	315111848,
	421295532,
	493193675,
	518613297,
	493193675,
	421295532,
	315111848,
	192197614,
	71964461,
	-28092393,
	-95623309,
	-125439197,
	-119932313,
	-87891547,
	-42059779,
	3953513,
	38677951,
	55039918,
	51418007,
	31349513
};


static q31_t firState1[DSP_BLOCK_SIZE + NUM_TAPS ];
static q31_t firState2[DSP_BLOCK_SIZE + NUM_TAPS ];

//arm_fir_instance_q31 S1;
//arm_fir_instance_q31 S2;

arm_fir_decimate_instance_q31 S1;
//arm_fir_decimate_instance_q31 S2;

bool bypassDSP = 1;

/*
void setPin(GPIO_TypeDef* port, uint16_t pin, uint8_t state){
	if (state)
		port->ODR |= (1<<pin);
	else
		port->ODR &= ~(1<<pin);
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/

/*
#define FFT_LEN	128
q31_t	fftData[FFT_LEN*2] = {0};
arm_cfft_instance_q31 fftS;
q31_t mags[FFT_LEN] = {0};

void fftInit(void) {
	arm_cfft_radix4_init_q15(&fftS, FFT_LEN, 0, 1);
	//arm_rfft_fast_init_f32(&fftR, FFT_LEN);
}
*/

int outStarted = 0;
void dspInit(void){
	outStarted = 0;
	//arm_fir_init_q31(&S1, NUM_TAPS, firCoeffs32, firState1, DSP_BLOCK_SIZE);
	//arm_fir_init_q31(&S2, NUM_TAPS, firCoeffs32, firState2, DSP_BLOCK_SIZE);

	arm_fir_decimate_init_q31(&S1, NUM_TAPS, 2, firCoeffs32, firState1, DSP_BLOCK_SIZE);
	//arm_fir_decimate_init_q31(&S1, NUM_TAPS, 2, firCoeffs32, firState1, DSP_BLOCK_SIZE);

}

void dspStart(void){
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)inBuf, ADC_BUFFER_LEN);
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t*)outBuf, ADC_BUFFER_LEN/2);
}


inline q31_t floatQ31(float f){
	float a = f;
	return (q31_t)(f * 2147483648.0f);
}

inline float Q31float(q31_t i){
	return ((float)i / 2147483648.0f);
}

/*
uint16_t	normalizedToPwmOut(float f){
	return ((int)(f * (float)PWM_HALF) + PWM_HALF);
}

float	adcToNormalized(uint16_t input){
	return (float)((float)(input - ADC_HALF)/(float)ADC_MAX);
}
*/


q31_t dspInI[DSP_BLOCK_SIZE];
q31_t dspInQ[DSP_BLOCK_SIZE];

q31_t dspOut[DSP_BLOCK_SIZE];


inline uint16_t q31toPwm(q31_t input){
	//int a = input;
	//int z = (a >> 22);
	//int x = z + PWM_HALF;
	return ((input >> 21) + PWM_HALF);
}

inline q31_t	adcToQ31(int16_t input){
	return ((input - ADC_HALF) << 20);
}

int start = 0;
int end  = 0;
int adcBuffStart = 0;

//int32_t debugInput[ADC_BUFFER_LEN] = {0xff};
//float debugInputF[ADC_BUFFER_LEN] = {555.0f};


void dspPrepareInput(void){
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,dspRingHalf);
	adcBuffStart = ADC_BUFFER_LEN/2 * (dspRingHalf == HALF_LOWER);

	for (int i=0; i < DSP_BLOCK_SIZE; i++){
		//debugInput[i] = adcToQ31(inBuf[i * 2 + adcBuffStart]);
		//debugInputF[i] = Q31float(adcToQ31(inBuf[i * 2 + adcBuffStart]));
		dspInI[i] = adcToQ31(inBuf[i * 2 + adcBuffStart]);
		dspInQ[i] = adcToQ31(inBuf[i * 2 + adcBuffStart + 1]);
	}

	//done = 1;
}

void dspPrepareOutput(void){
	adcBuffStart = ADC_BUFFER_LEN/4 * (dspRingHalf == HALF_LOWER);
	// если здесь /2, частота будет ниже

	for (int k=0; k < DSP_BLOCK_SIZE; k++){
		// до децимации было outBuf[adcBuffStart + k] = 		q31toPwm(dspOut[k]);
		outBuf[adcBuffStart + k * 2] = 		q31toPwm(dspOut[k]);
		outBuf[adcBuffStart + k * 2 + 1] = 	q31toPwm(dspOut[k]);
	}



}

#include <stdlib.h>

q31_t softClip(q31_t* source, q31_t* destination, uint16_t blockSize){
	//in = in - 0.33333333333333f * in * in * in;
	//if (in > 0.999) in = 0.999;
	//if (in < -0.999) in = -0.999;

	q31_t multResult[blockSize];
	q31_t cilpA = FtoQ31(0.333333333f);

	arm_mult_q31(source, source, multResult, blockSize); // in^2
	arm_mult_q31(source, multResult, multResult, blockSize); // in^3
	arm_scale_q31(multResult, cilpA, 0, multResult, blockSize); // in^3 * 1/3
	//arm_negate_q31(multResult, multResult, blockSize); // - (in^3 * 1/3)
	//arm_add_q31(source, multResult, destination, blockSize); // in - (in^3 * 1/3)
	arm_sub_q31(source, multResult, destination, blockSize); // in - (in^3 * 1/3)

	/*
	for (uint16_t i=0; i<blockSize; i++){
		q31_t input = source[i];


		q31_t cube = 0;

		arm_mult_q31(&input, &input, &cube, 1); // ^2
		arm_mult_q31(&input, &cube, &cube, 1); // ^3
		arm_mult_q31(&cilpA, &cube, &cube, 1); // ^3 * 1/3

		destination[i] = input - cube;

	}
	*/
}

q31_t sinX = 0;
void dspProc(void){

	if (!dspProcDone){

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, dspRingHalf);

		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);

		//arm_scale_f32(blockIN, (1.0f/4096.0f), blockOUT, BUF_LEN/2);

		dspPrepareInput();
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
			//arm
			//arm_scale_q31(dspInI, floatQ31(1.0), 0, dspOut, BUF_LEN/2);
			//arm_scale_q31(dspInI, floatQ31(2.0), 0, dspOut, BUF_LEN/2);
			//arm_copy_q31(dspInI, dspOut, DSP_BLOCK_SIZE);


			//arm_cfft_q31(&fftS, blockOUT, 0, 1);
			fftProcess(dspOut);

			// main selection filter
			//arm_fir_decimate_q31(&S1, dspInQ, dspOut, DSP_BLOCK_SIZE);
			arm_fir_decimate_q31(&S1, dspInI, dspOut, DSP_BLOCK_SIZE);

			//arm_fir_q31(&S1, dspInQ, dspOut, DSP_BLOCK_SIZE);
			//arm_fir_q31(&S2, dspInI, dspOut, DSP_BLOCK_SIZE);




			//for (int z=0;z<DSP_BLOCK_SIZE; z++){
				//q31_t in = ;
				agcPrasolovFloat(dspOut, dspOut, DSP_BLOCK_SIZE);
				softClip(dspOut, dspOut, DSP_BLOCK_SIZE);

				//dspOut[z] = softClip(dspOut[z]);
			//}


			//float agcBuf[DSP_BLOCK_SIZE];
			//arm_q31_to_float(dspOut, agcBuf, DSP_BLOCK_SIZE);
			//AudioAgc_RunAgcWdsp(DSP_BLOCK_SIZE, agcBuf, 0);

/*
			for (int z=0;z<DSP_BLOCK_SIZE; z++){
				sinX += floatQ31(0.0078125f);
				if (sinX > floatQ31(0.9999f))
					sinX = 0;
				dspOut[z] = arm_sin_q31(sinX);
				//dspOut[z] = 0 + (z>DSP_BLOCK_SIZE/2)*floatQ31(0.9f);

			}
*/


			//arm_q15_to_float(blockOUT, a, BUF_LEN/2);

		dspPrepareOutput();
		dspProcDone = 1;
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc1){

}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1){



}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim1){
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);


	dspRingHalf = HALF_UPPER;
	dspProcDone = 0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,dspRingHalf);
	elseDone = 0;
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, dspRingHalf);
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

};


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim1){
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);


	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, dspRingHalf);
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);

		dspRingHalf = HALF_LOWER;
		dspProcDone = 0;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,dspRingHalf);
		elseDone = 0;
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, dspRingHalf);
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

}
