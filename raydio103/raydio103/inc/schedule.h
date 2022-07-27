#pragma once

#include "arm_math.h"
#include "dsp.h"
#include <stdint.h>
#include <stdbool.h>


void schedInit(void);
extern  inline void schedMainLoop(void);

