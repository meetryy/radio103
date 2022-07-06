#pragma once

#include "dsp.h"
#include "arm_math.h"
#include <stdint.h>

q31_t softClip(q31_t* source, q31_t* destination, uint16_t blockSize);

