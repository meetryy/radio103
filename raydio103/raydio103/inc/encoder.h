#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "global.h"

enum encNames {ENC_UPPER, ENC_LOWER, ENC_NR};
typedef struct {
	volatile uint8_t 			phaseState;
	volatile uint8_t 			phaseOldState;
	volatile int				deltaRaw;
	//int						deltaOld;
	volatile int					delta;
	volatile bool 					isReversed;
	volatile uint8_t				divider;
} encoder_t;

extern encoder_t encoder[ENC_NR];

void encInputProcess(int encNum, bool a, bool b);

