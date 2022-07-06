#include "global.h"
#include "dsp.h"
#include <stdint.h>
#include "global.h"
#include "main.h"
#include "arm_math.h"
#include "fft.h"
#include "agc.h"
#include "fir.h"
#include "audio.h"
#include <stdbool.h>

enum dspRingH {HALF_LOWER, HALF_UPPER};
volatile bool dspRingHalf = HALF_LOWER;
volatile bool dspProcDone = 0;

uint16_t  inBuf[ADC_BUFFER_LEN] = {0};
uint16_t  outBuf[ADC_BUFFER_LEN/ADC_DMA_CHANNELS] = {0};

bool bypassDSP = 1;


const q31_t firCoeffs[FIR_NUM_TAPS] = {
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

static q31_t firStateI[DSP_BLOCK_SIZE + FIR_NUM_TAPS + 1];
static q31_t firStateQ[DSP_BLOCK_SIZE + FIR_NUM_TAPS + 1];

arm_fir_instance_q31 firInstanceI;
arm_fir_instance_q31 firInstanceQ;

arm_fir_decimate_instance_q31	firInstanceDecI;
arm_fir_decimate_instance_q31	firInstanceDecQ;

int outStarted = 0;
void dspInit(void){
	outStarted = 0;
	firInit();

	arm_fir_init_q31(&firInstanceI, FIR_NUM_TAPS, firCoeffs, firStateI, DSP_BLOCK_SIZE);
	arm_fir_init_q31(&firInstanceQ, FIR_NUM_TAPS, firCoeffs, firStateQ, DSP_BLOCK_SIZE);

	arm_fir_decimate_init_q31(&firInstanceDecI, FIR_NUM_TAPS, 2, firCoeffs, firStateI, DSP_BLOCK_SIZE);
	//arm_fir_decimate_init_q31(&firInstanceDecQ, FIR_NUM_TAPS, 2, firCoeffs, firStateQ, DSP_BLOCK_SIZE);


	 fftInit();
	//arm_fir_init_q31(&S1, NUM_TAPS, firCoeffs32, firState1, DSP_BLOCK_SIZE);
	//arm_fir_init_q31(&S2, NUM_TAPS, firCoeffs32, firState2, DSP_BLOCK_SIZE);

	//arm_fir_decimate_init_q31(&S1, NUM_TAPS, 2, firCoeffs32, firState1, DSP_BLOCK_SIZE);
	//arm_fir_decimate_init_q31(&S1, NUM_TAPS, 2, firCoeffs32, firState1, DSP_BLOCK_SIZE);
	 outStarted = 1;

}

void dspStart(void){
	HAL_TIM_Base_Start(&htim1);
	//HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // TIM1 CH2 = ADC trigger
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)inBuf, ADC_BUFFER_LEN); // CH3 = audio output
																	// ADC_BUFFER_LEN/(ADC_DMA_CHANNELS)
	//HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t*)outBuf, ADC_BUFFER_LEN/(ADC_DMA_CHANNELS)/2);

	#ifdef DSP_DECIMATED
		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t*)outBuf, ADC_BUFFER_LEN/(ADC_DMA_CHANNELS));
	#endif

	#ifndef DSP_DECIMATED
		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t*)outBuf, ADC_BUFFER_LEN/(ADC_DMA_CHANNELS));
	#endif

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

int start = 0;
int end  = 0;
//int adcBuffStart = 0;

void dspPrepareInput(void){
	int adcBuffStart = ADC_BUFFER_LEN/2 * (dspRingHalf == HALF_LOWER);

	for (int i=0; i < DSP_BLOCK_SIZE; i++){
		int ptr = i * ADC_DMA_CHANNELS + adcBuffStart;
		dspInI[i] = adcToQ31(inBuf[ptr]);
		dspInQ[i] = adcToQ31(inBuf[ptr + 1]);
	}
}

void dspPrepareOutput(void){

#ifdef DSP_DECIMATED
	int adcBuffStart = ADC_BUFFER_LEN/(ADC_DMA_CHANNELS*2) * (dspRingHalf == HALF_LOWER);

	for (int k=0; k < DSP_BLOCK_SIZE/2; k++){
		int ptr = adcBuffStart + k*2;
		outBuf[ptr] = 		q31toPwm(dspOut[k]);
		outBuf[ptr + 1] = 	q31toPwm(dspOut[k]);
	}
#endif

#ifndef DSP_DECIMATED
	int adcBuffStart = ADC_BUFFER_LEN/(ADC_DMA_CHANNELS*2) * (dspRingHalf == HALF_LOWER);

	for (int k=0; k < DSP_BLOCK_SIZE; k++){
		outBuf[adcBuffStart + k] = 		q31toPwm(dspOut[k]);
	}
#endif

}

q31_t sinX = 0;
void dspProc(void){

	if (!dspProcDone){

		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
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

#ifdef DSP_DECIMATED
			arm_fir_decimate_q31(&firInstanceDecI, dspInI, dspOut, DSP_BLOCK_SIZE);
			//arm_fir_decimate_q31(&firInstanceDecQ, dspInQ, dspOut, DSP_BLOCK_SIZE);
#endif

#ifndef DSP_DECIMATED
			arm_fir_q31(&firInstanceI, dspInI, dspOut, DSP_BLOCK_SIZE);
			//arm_fir_q31(&firInstanceQ, dspInQ, dspOut, DSP_BLOCK_SIZE);
#endif

				agcPrasolovFloat(dspOut, dspOut, DSP_BLOCK_SIZE_DEC);
				softClip(dspOut, dspOut, DSP_BLOCK_SIZE_DEC);


		dspPrepareOutput();
		dspProcDone = 1;
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);

		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
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
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,dspRingHalf);
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
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,dspRingHalf);
		elseDone = 0;
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, dspRingHalf);
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

}
