#include "metrics.h"
#include "global.h"
#include "main.h"

metricsInfo_t metrics = {
		.metric[METRIC_DSP_PREP_IN] = {"prep in", 0, 0.0f},
		.metric[METRIC_DSP_FFT] = {"fft", 0, 0.0f},
		.metric[METRIC_DSP_FIR] = {"fir", 0, 0.0f},
		.metric[METRIC_DSP_AGC] = {"agc", 0, 0.0f},
		.metric[METRIC_DSP_CLIP] = {"clip", 0, 0.0f},
		.metric[METRIC_DSP_PREP_OUT] = {"prep out", 0, 0.0f},
		.metric[METRIC_DSP_TOTAL] = {"total", 0, 0.0f},
};

void metricsSet(int ID, uint32_t data){
	metrics.metric[ID].time = data;
};
