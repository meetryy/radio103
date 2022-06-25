#ifndef INC_DSP_H_
#define INC_DSP_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

#define	ADC_DMA_CHANNELS	2
#define ADC_BUFFER_LEN		512
#define AUDIO_BUFFER_LEN	(ADC_BUFFER_LEN/ADC_DMA_CHANNELS) 			// buffer is I+Q, so DSP block size is BUF_LEN/2
#define DSP_BLOCK_SIZE		(AUDIO_BUFFER_LEN/2)
#define	DSP_SAMPLING_FREQ	((72000000/1024/2)/2) // /2 since buffer is I+Q

#define ADC_BITS			12
#define ADC_MAX				((1<<ADC_BITS))
#define ADC_HALF			(ADC_MAX/2)

#define PWM_BITS			10
#define	PWM_MAX				((1 << PWM_BITS))
#define PWM_HALF			(PWM_MAX/2)

#define INT_GAIN			10

extern uint16_t  outBuf[ADC_BUFFER_LEN];
extern uint16_t  inBuf[ADC_BUFFER_LEN];

void dspProc(void);
void dspInit(void);
void fftInit(void);
void dspStart(void);

extern int outStarted;


#endif /* INC_DSP_H_ */
