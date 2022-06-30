#pragma once

#include "arm_math.h"
#include "dsp.h"
#include <stdint.h>
#include <stdbool.h>
void compInit(void);
q31_t compProcess (q31_t inSample);
q31_t compProcessFloat (q31_t inSample);
//q31_t agcPrasolov(q31_t in);
void agcPrasolov(q31_t* source, q31_t* destination, uint16_t blockSize);
void agcPrasolovFloat(q31_t* source, q31_t* destination, uint16_t blockSize);

typedef float float32_t;

// these are the control parameter
// for external runtime configuration
// of the AGC
typedef struct
{
    uint8_t mode;
    uint8_t slope;
    uint8_t hang_enable;
    int     thresh;
    int     hang_thresh;
    int 	hang_time;
    uint8_t action;
    uint8_t switch_mode;
    uint8_t hang_action;
    int 	tau_decay[6];
    int 	tau_hang_decay;
} agc_wdsp_params_t;

extern agc_wdsp_params_t agc_wdsp_conf;

#define ADC_CLIP_WARN_THRESHOLD 4800
#define AGC_WDSP_RB_SIZE 		((DSP_SAMPLING_FREQ/1000)*4)
// max buffer size based on max sample rate to be supported
// this translates to 192 at 48k SPS. We have FM using the AGC at full sampling speed

extern float gaine;
extern float fInput;
