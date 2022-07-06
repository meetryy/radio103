#include "global.h"
#include "main.h"
#include "dsp.h"
#include "audio.h"
#include "arm_math.h"
#include <stdbool.h>
#include <stdint.h>

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

