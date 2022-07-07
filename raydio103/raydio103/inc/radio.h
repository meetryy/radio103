#pragma once
#include <stdint.h>
#include <stdbool.h>

enum mods{MOD_LSB, MOD_USB, MOD_CW, MOD_CWN, MOD_AM, MOD_FM, MOD_NR};
enum vfos{VFO_A, VFO_B, VFO_NR};
enum bands{BAND_40M, BAND_30M, BAND_20M, BAND_NR};
enum agcs{AGC_FAST, AGC_SLOW, AGC_MID, AGC_NR};

typedef struct{
	// modulation-specific parameters

	bool 		agcOn;
	int			agcID;
	float		agcAlpha[AGC_NR];
	int			vfoStepHz;
	int			vfoStepFastHz;
} modState_t;

typedef struct{
	//band-specific parameters

	uint32_t	vfoFreqHz[VFO_NR];
	uint32_t	vfoFreqLimits[2];
	bool		vfoCurrent;

	int			modulation[VFO_NR];
	int			rxFilterID[VFO_NR];
} bandState_t;

typedef struct{
	uint32_t	rxFreqHz;
	uint32_t	txFreqHz;

	int			vfoStepHz;
	int			vfoStepFastHz;

	int			rxMod;
	int 		txMod;

	int			rxFilterID;

	bool		txState;

	uint32_t	sMeterADC;
	float		sMeterdBm;

	float		swrFadc;
	float		swrRadc;
	float		SWR;

	float		battVoltage;
	uint32_t	battADC;
	float		battADCcoeff;

	bool 		agcOn;
	int			agcID;
	float		agcAlpha[AGC_NR];

	int			bandID;
	bandState_t bandData[BAND_NR];

	int			modID;
	modState_t	modData[MOD_NR];

} radioState_t;

extern radioState_t radio;


