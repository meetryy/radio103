#include "fft.h"
#include "global.h"

#include <stdint.h>

arm_cfft_instance_q31 fftS;

void fftInit(void) {
	//arm_cfft_radix4_init_q31(&fftS, FFT_LEN, 0, 1);
}

q31_t fftBuf[FFT_LEN*2];
q31_t magnitudes[FFT_LEN/2];

void fftProcess(q31_t *inputF32) {
	//arm_cfft_radix4_q31(&fftS, inputF32);

	arm_copy_q31(inputF32, fftBuf, FFT_LEN);
	arm_cfft_q31(&fftS, fftBuf, 0, 1);
	arm_cmplx_mag_q31(fftBuf, magnitudes, FFT_LEN);

	arm_scale_q31(magnitudes, 2126008812, -24, magnitudes, FFT_LEN/2);



	//arm_max_f32(fftMagnitudesdB, FFT_LEN, &maxValue, &maxIndex);
}






