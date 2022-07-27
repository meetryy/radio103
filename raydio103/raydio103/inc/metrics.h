#pragma once
#include <stdint.h>
#include <stdbool.h>

enum metricNames {
	METRIC_DSP_START,
	METRIC_DSP_PREP_IN,
	METRIC_DSP_FFT1,
	METRIC_DSP_FFT2,
	METRIC_DSP_FIR,
	METRIC_DSP_AGC,
	METRIC_DSP_CLIP,
	METRIC_DSP_PREP_OUT,
	METRIC_DSP_TOTAL,
	METRIC_GFX_START,
	METRIC_GFX_TOTAL,
	METRIC_ELSE_START,
	METRIC_ELSE_TOTAL,
	METRIC_NR};

typedef struct{
	char 		name[16];
	uint32_t 	time;
	float		fraction;
} metric_t;


typedef struct{
	metric_t metric[METRIC_NR];
} metricsInfo_t;

extern metricsInfo_t metrics;

void metricsSet(int ID, uint32_t data);
