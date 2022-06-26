#pragma once

#include "dsp.h"
#include "arm_math.h"
#include <stdint.h>

#define FFT_LEN			(128)
#define FFT_USEFUL_BINS	(FFT_LEN/2)
#define FFT_HZ_PER_BIN	(AUDIO_BUFFER_HALF/FFT_LEN)

extern q31_t magnitudes[FFT_LEN/2];

void fftProcess(q31_t *inputF32);
void fftInit(void);
