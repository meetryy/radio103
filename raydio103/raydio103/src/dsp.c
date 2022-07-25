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
#include "global.h"
#include "radio.h"
#include <stdbool.h>
#include "metrics.h"

enum dspRingH {HALF_LOWER, HALF_UPPER};
volatile bool dspRingHalf = HALF_LOWER;
volatile bool dspProcDone = 0;

uint16_t  inBuf[ADC_BUFFER_LEN] = {0};
uint16_t  outBuf[ADC_BUFFER_LEN/ADC_DMA_CHANNELS] = {0};

bool bypassDSP = 0;

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

const q31_t firCoeffsInter[FIR_NUM_TAPS_INTER] = {
	3050363,
	-3046247,
	3041618,
	2144446145,
	3041618,
	-3046247,
	3050363,
	-3053966
};

static q31_t firStateI[DSP_BLOCK_SIZE + FIR_NUM_TAPS + 1];
static q31_t firStateQ[DSP_BLOCK_SIZE + FIR_NUM_TAPS + 1];
static q31_t firStateInter[DSP_BLOCK_SIZE + FIR_NUM_TAPS_INTER + 1];

arm_fir_instance_q31 firInstanceI;
arm_fir_instance_q31 firInstanceQ;

arm_fir_decimate_instance_q31	firInstanceDecI;
arm_fir_decimate_instance_q31	firInstanceDecQ;
arm_fir_interpolate_instance_q31 firInstanceInter;

int outStarted = 0;
void dspInit(void){
	outStarted = 0;
	firInit();


#ifdef DSP_DECIMATED
	arm_fir_decimate_init_q31(&firInstanceDecI, 	FIR_NUM_TAPS, 			DSP_DECIMATION_RATE, 	firCoeffs, 		firStateI, 		DSP_BLOCK_SIZE);
	arm_fir_decimate_init_q31(&firInstanceDecQ, 	FIR_NUM_TAPS, 			DSP_DECIMATION_RATE, 	firCoeffs, 		firStateQ,		DSP_BLOCK_SIZE);
	arm_fir_interpolate_init_q31(&firInstanceInter, DSP_DECIMATION_RATE, 	FIR_NUM_TAPS_INTER, 	firCoeffsInter, firStateInter, 	DSP_BLOCK_SIZE);
	#endif

#ifndef DSP_DECIMATED
	arm_fir_init_q31(&firInstanceI, FIR_NUM_TAPS, firCoeffs, firStateI, DSP_BLOCK_SIZE);
	arm_fir_init_q31(&firInstanceQ, FIR_NUM_TAPS, firCoeffs, firStateQ, DSP_BLOCK_SIZE);
#endif

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

	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t*)outBuf, ADC_BUFFER_LEN/(ADC_DMA_CHANNELS));
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


__attribute__((optimize("unroll-loops"))) void dspPrepareInput(void) {
	int adcBuffStart = ADC_BUFFER_LEN/2 * (dspRingHalf == HALF_LOWER);

	//__attribute__(optimize("unroll-loops"))
	 for (int i=0; i < DSP_BLOCK_SIZE; i++){
		int ptr = i * ADC_DMA_CHANNELS + adcBuffStart;
		dspInI[i] = adcToQ31(inBuf[ptr]);
		dspInQ[i] = adcToQ31(inBuf[ptr + 1]);
	}
}

/*
void dspPrepareInputUnrolled(void){
	int adcBuffStart = ADC_BUFFER_LEN/2 * (dspRingHalf == HALF_LOWER);

	dspInI[0] = adcToQ31(inBuf[0 + adcBuffStart]);
	dspInI[1] = adcToQ31(inBuf[0 + adcBuffStart]);
	dspInI[2] = adcToQ31(inBuf[0 + adcBuffStart]);
	dspInI[3] = adcToQ31(inBuf[0 + adcBuffStart]);
	dspInI[4] = adcToQ31(inBuf[0 + adcBuffStart]);
	dspInI[5] = adcToQ31(inBuf[0 + adcBuffStart]);
	dspInI[6] = adcToQ31(inBuf[0 + adcBuffStart]);
	dspInI[7] = adcToQ31(inBuf[0 + adcBuffStart]);
}
*/

__attribute__((optimize("unroll-loops"))) void dspPrepareOutput(void){

	#ifdef DSP_DECIMATED_NO_INTERPOLATION
			int adcBuffStart = ADC_BUFFER_LEN/(ADC_DMA_CHANNELS*2) * (dspRingHalf == HALF_LOWER);

			for (int k=0; k < DSP_BLOCK_SIZE/2; k++){
				int ptr = adcBuffStart + k*2;
				outBuf[ptr] = 		q31toPwm(dspOut[k]);
				outBuf[ptr + 1] = 	q31toPwm(dspOut[k]);
			}

	#else
		int adcBuffStart = ADC_BUFFER_LEN/(ADC_DMA_CHANNELS*2) * (dspRingHalf == HALF_LOWER);

		for (int k=0; k < DSP_BLOCK_SIZE; k++)
			outBuf[adcBuffStart + k] = 		q31toPwm(dspOut[k]);
	#endif

}

float ssbLOgenX = 0;
float Xmax = FtoQ31(0.9999f);
const float LOfreq = 6000.0f; //1000.0f;
float Xinc = (LOfreq/(float)DSP_SAMPLING_FREQ)*4.0f*M_PI;

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

uint32_t getTimeDiff(void){
	//static uint32_t lastTime;
	uint32_t time = __HAL_DMA_GET_COUNTER(&hdma_adc1);

	//uint32_t r =  (lastTime - time);
	//lastTime = time;
	return time;
}

void setTime(int i){
	//metricsSet(i, __HAL_DMA_GET_COUNTER(&hdma_adc1));
	metrics.metric[i].time = __HAL_DMA_GET_COUNTER(&hdma_adc1);
}

//__attribute__ ((section(".RamFunc")))
void dspProc(void){
	if (!dspProcDone){
		setTime(METRIC_DSP_START);
		if(radio.txState == RX){
		// receive

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);

			// fill dspInI[], dspInQ
			dspPrepareInput();

			setTime(METRIC_DSP_PREP_IN);

			// process FFT
			//fftProcess(dspOut);
			setTime(METRIC_DSP_FFT);

			q31_t processingBufferI[DSP_BLOCK_SIZE_DEC];
			q31_t processingBufferQ[DSP_BLOCK_SIZE_DEC];

			// main filter
		#ifdef DSP_DECIMATED
			arm_fir_decimate_q31(&firInstanceDecI, dspInI, dspOut, DSP_BLOCK_SIZE);
			arm_fir_decimate_q31(&firInstanceDecQ, dspInQ, processingBufferQ, DSP_BLOCK_SIZE);

		#else
			arm_fir_q31(&firInstanceI, dspInI, dspOut, DSP_BLOCK_SIZE);
			arm_fir_q31(&firInstanceQ, dspInQ, dspOut, DSP_BLOCK_SIZE);
		#endif

			setTime(METRIC_DSP_FIR);
			agcPrasolovFloat(dspOut, dspOut, DSP_BLOCK_SIZE_DEC);
			setTime(METRIC_DSP_AGC);
			softClip(dspOut, dspOut, DSP_BLOCK_SIZE_DEC);
			setTime(METRIC_DSP_CLIP);
			/*
			for (int z=0; z < DSP_BLOCK_SIZE_DEC; z++){

				ssbLOgenX += Xinc;

				if (ssbLOgenX > ((float)M_PI*2.0f))
					ssbLOgenX -= ((float)M_PI*2.0f);

				float s = arm_sin_f32(ssbLOgenX);
				dspOut[z] = FtoQ31(s);



			}
	*/

		#ifndef DSP_DECIMATED_NO_INTERPOLATION
			arm_fir_interpolate_q31(&firInstanceInter, processingBufferI, dspOut, DSP_BLOCK_SIZE_DEC);
		#endif

			dspPrepareOutput();
			setTime(METRIC_DSP_PREP_OUT);

		}

		else{
			// transmit DSP
		}
		setTime(METRIC_DSP_TOTAL);
		dspProcDone = 1;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc1){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,dspRingHalf);
		dspRingHalf = HALF_UPPER;
		dspProcDone = 0;
		elseDone = 0;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,dspRingHalf);
		dspRingHalf = HALF_LOWER;
		dspProcDone = 0;
		elseDone = 0;
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim1){

};


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim1){

}
