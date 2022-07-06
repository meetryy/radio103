#include "global.h"
#include "main.h"
#include "dsp.h"
#include "fir.h"
#include "arm_math.h"
#include <stdbool.h>
#include <stdint.h>


//arm_fir_decimate_instance_q31 firInstanceDecI;
//arm_fir_decimate_instance_q31 firInstanceDecQ;

void firInit(void){


	//arm_fir_decimate_init_q31(&firInstanceI, FIR_NUM_TAPS, 2, firCoeffs, firStateI, DSP_BLOCK_SIZE);
	//arm_fir_decimate_init_q31(&firInstanceQ, FIR_NUM_TAPS, 2, firCoeffs, firStateQ, DSP_BLOCK_SIZE);

}

enum filterType {FILTER_MAIN_I, FILTER_MAIN_Q};

void firProc(int filter, q31_t* inPtr, q31_t* outPtr){

	// main selection filter
	switch (filter){
		case FILTER_MAIN_I:
			arm_fir_q31(&firInstanceI, inPtr, outPtr, DSP_BLOCK_SIZE);
			break;

		case FILTER_MAIN_Q:
			arm_fir_q31(&firInstanceQ, inPtr, outPtr, DSP_BLOCK_SIZE);
			break;

	}

	//arm_fir_decimate_q31(&firInstanceI, dspInI, dspOut, DSP_BLOCK_SIZE);
	//arm_fir_decimate_q31(&firInstanceQ, dspInQ, dspOut, DSP_BLOCK_SIZE);

	//arm_fir_q31(&S1, dspInQ, dspOut, DSP_BLOCK_SIZE);
	//arm_fir_q31(&S2, dspInI, dspOut, DSP_BLOCK_SIZE);
}


