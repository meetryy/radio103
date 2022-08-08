#include "arm_math.h"

#define FIR_NUM_TAPS		(32)
#define FIR_NUM_TAPS_INTER	(8)

extern arm_fir_decimate_instance_q31	firInstanceDecI;
extern arm_fir_decimate_instance_q31	firInstanceDecQ;

extern arm_fir_instance_q31 			firInstanceI;
extern arm_fir_instance_q31 			firInstanceQ;

void firInit(void);
void firProc(int filter, q31_t* inPtr, q31_t* outPtr);

