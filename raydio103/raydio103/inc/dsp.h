#ifndef INC_DSP_H_
#define INC_DSP_H_

#include <stdbool.h>
#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "arm_math.h"

#define DSP_DECIMATED
#define DSP_DECIMATION_RATE	(2)
#define 	DSP_DECIMATED_NO_INTERPOLATION

#define	ADC_DMA_CHANNELS	(2)
#define ADC_BUFFER_LEN		(512)
#define AUDIO_BUFFER_LEN	(ADC_BUFFER_LEN/ADC_DMA_CHANNELS) 			// buffer is [1][2][3][4][1][2][3][4]
#define DSP_BLOCK_SIZE		(AUDIO_BUFFER_LEN/2)
#define DSP_BLOCK_SIZE_DEC	(DSP_BLOCK_SIZE/2)
#define	DSP_SAMPLING_FREQ	((128000000/(1 << PWM_BITS)/2)/ADC_DMA_CHANNELS) // /2 since buffer is I+Q

#define ADC_BITS			(12)
#define ADC_MAX				(1<<ADC_BITS)
#define ADC_HALF			(ADC_MAX/2)

#define PWM_BITS			11	// 2048
//#define PWM_BITS			10	// 1024

// FIXME: add check ((1 << PWM_BITS) == TIM1->ARR) at DSP init

#define	PWM_MAX				(1 << PWM_BITS)
#define PWM_HALF			(PWM_MAX/2)

#define INT_GAIN			(10)

#define FtoQ31(x) 			(q31_t)(x * 2147483648.0f) //2147483648.0f
#define Q31toF(x) 			(float)(x / 2147483648.0f)	//2147483648.0f
//#define q31ToPwm(x)			(uint16_t)((x >> 21) + PWM_HALF)
//#define adcToQ31(x)			(q31_t)((x - ADC_HALF) << 20)
inline uint16_t q31toPwm(q31_t input)	{return ((input >> 21) + PWM_HALF);}
inline q31_t	adcToQ31(int16_t input)	{return ((input - ADC_HALF) << 20);}


extern uint16_t  outBuf[ADC_BUFFER_LEN/ADC_DMA_CHANNELS]; // зачем-то надо делить на количество каналов, ХЗ
extern uint16_t  inBuf[ADC_BUFFER_LEN];

void dspProc(void);
void dspInit(void);
void fftInit(void);
void dspStart(void);

extern int outStarted;
extern q31_t dspInI[DSP_BLOCK_SIZE];
extern q31_t dspInQ[DSP_BLOCK_SIZE];

extern q31_t dspOut[DSP_BLOCK_SIZE];

#define FFT_LEN			(256)
#define FFT_USEFUL_BINS	(FFT_LEN/2)
#define FFT_HZ_PER_BIN	(DSP_SAMPLING_FREQ/FFT_USEFUL_BINS)

extern volatile q31_t magnitudes[FFT_USEFUL_BINS];

extern int dspLoad;
extern bool dspProcDone;

#endif /* INC_DSP_H_ */
