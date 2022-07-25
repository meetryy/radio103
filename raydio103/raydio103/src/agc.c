#include "agc.h"
#include "dsp.h"
#include "global.h"

/*
enum CompStates {S_NoOperation, S_Attack, S_GainReduction, S_Release };
enum CompStates State = S_NoOperation;

int32_t Attack, Release, Hold, TimeOut;
float gainreduce, gain_step_attack, gain_step_release, gain, threshold;




void compInit(void){
	// 1 sample = 1/96kHz = ~10us
	//Attack -> 30 ms -> 3000
	//Release -> 20 ms -> 2000
	//Hold -> 10ms -> 1000
	Attack = 3000;
	Release = 2000;
	Hold = 1000;

	//threshold -20dB below limit -> 0.1 * 2^31
	threshold = (0.01f * 2147483648);

	//compression ratio: 6:1 -> -6dB = 0.5
	gainreduce = (0.5f);

	gain_step_attack = ((1.0f - gainreduce) / Attack);
	gain_step_release = ((1.0f - gainreduce) / Release);

	//initial gain = 1.0 -> no compression
	gain = (0.5f);
}


q31_t compProcessFloat (q31_t inSample) {
	float inSampleF = Q31float(inSample);

	if (fabs(inSampleF) > threshold) {
		if (gain >=  gainreduce) {
			if (State == S_NoOperation) {
				State = S_Attack;
				TimeOut = Attack;
			}
			else if (State == S_Release) {
				State = S_Attack;
				TimeOut = Attack;
			}
		}
		if (State == S_GainReduction)
			TimeOut = Hold;
	}

	if (fabs(inSampleF) < threshold && gain <= 1.0f) {
		if (TimeOut == 0 && State == S_GainReduction) {
			State = S_Release;
			TimeOut = Release;
		}
	}

	switch (State) {
		case S_Attack:
			if (TimeOut > 0 && gain > gainreduce) {
				gain -= gain_step_attack;
				TimeOut--;
			}
			else {
				State=S_GainReduction;
				TimeOut = Hold;
			}
			break;

		case S_GainReduction:
			if (TimeOut>0) TimeOut--;
			else {
				State=S_Release;
				TimeOut = Release;
			}
			break;

		case S_Release:
			if (TimeOut>0 && gain<1.0f) {
				TimeOut--;
				gain += gain_step_release;
			}
			else
				State=S_NoOperation;
			break;

		case S_NoOperation:
			if (gain < 1.0f)
				gain = 1.0f;
			break;

		default:
			break;
	}

	float outSampleF = inSampleF * gain;

	//return (int) outSampleF;
	return (floatQ31(outSampleF));
}



q31_t compProcess (q31_t inSample) {
	q31_t inSampleF = (q31_t)inSample;

	if (abs(inSampleF) > threshold) {
		if (gain >=  gainreduce) {
			if (State == S_NoOperation) {
				State = S_Attack;
				TimeOut = Attack;
			}
			else if (State == S_Release) {
				State = S_Attack;
				TimeOut = Attack;
			}
		}
		if (State == S_GainReduction)
			TimeOut = Hold;
	}

	if (abs(inSampleF) < threshold && gain <= floatQ31(1.0f)) {
		if (TimeOut == 0 && State == S_GainReduction) {
			State = S_Release;
			TimeOut = Release;
		}
	}

	switch (State) {
		case S_Attack:
			if (TimeOut > 0 && gain > gainreduce) {
				gain -= gain_step_attack;
				TimeOut--;
			}
			else {
				State = S_GainReduction;
				TimeOut = Hold;
			}
			break;

		case S_GainReduction:
			if (TimeOut>0) TimeOut--;
			else {
				State = S_Release;
				TimeOut = Release;
			}
			break;

		case S_Release:
			if (TimeOut > 0 && gain < floatQ31(1.0f)) {
				TimeOut--;
				gain += gain_step_release;
			}
			else
				State = S_NoOperation;
			break;

		case S_NoOperation:
			if (gain < floatQ31(1.0f))
				gain = floatQ31(1.0f);
			break;

		default:
			break;
	}

	q31_t outSampleF = inSample * gain;
	return (q31_t) outSampleF;
}

*/

inline q31_t floatQ31(float f){
	//float a = f;
	return (q31_t)(f * 2147483648.0f);
}

inline float Q31float(q31_t i){
	return ((float)i / 2147483648.0f);
}

q31_t 	currentRatio;
bool 	attackphase = 0;
bool 	releasephase = 0;
int 	holdcount = 0;

/*
float 	attack = 0.1f;
float 	release = 0.999999f;
float 	threshold = 1.0f;
float 	ratio = 1.0f;
*/

q31_t 	attack = 2147484;  //0.001
q31_t 	release = 2145336164; // 0.999
q31_t 	threshold = 644245094; // 0.3
q31_t 	ratio =  1073741824; //0.5

#define Q31_1	(2147483648)


float logf_fast_ub(float a) {
  union { float f; int x; } u = { a };
  return (u.x - 1064631197) * 8.262958405176314e-8f;
}

float gaine = 0;
float fInput =0;
q31_t inAbs = 0;

q31_t maxiCompr(q31_t input) {
	//q31_t input = Q31float(in);
	q31_t output = 0;

	arm_abs_q31(&input, &inAbs, 1);

	//if (fabs(input)>threshold && attackphase!=1){
	if (inAbs > threshold && !attackphase){
		holdcount = 0;
		releasephase = 0;
		attackphase = 1;
		if (currentRatio == 0)
			currentRatio = ratio;
	}

	// if (attackphase==1 && currentRatio<ratio-1) {
	if (attackphase && (currentRatio < (ratio - Q31_1))) {
		//currentRatio*=(1+attack);
		q31_t a = currentRatio;
		q31_t b = (Q31_1 + attack);
		q31_t res = 0;
		arm_mult_q31(&a, &b, &res, 1);
		currentRatio = res;
	}

	if (currentRatio >= (ratio - 1)) {
		attackphase = 0;
		releasephase = 1;
	}

	if (releasephase && currentRatio > 0) {
		//currentRatio*=release;
		q31_t a = currentRatio;
		q31_t b = release;
		q31_t res = 0;
		arm_mult_q31(&a, &b, &res, 1);
		currentRatio = res;
	}

	fInput = Q31float(input);
	float fRatio = Q31float(ratio);
	float fCurrRatioPlusOne = Q31float(currentRatio + Q31_1 - 1);
	gaine = Q31float(currentRatio);

	//q31_t first; // == (input / (currentRatio+1))


	float fOut = (fInput / fCurrRatioPlusOne) * (1 + logf_fast_ub(fRatio));

	/*

	output = input / (1. + currentRatio);
	float out =  output*(1+log(ratio));

	float fOut = (input / (1.0f + currentRatio)) * (1+log(ratio));
	*/

	return floatQ31(fOut);
}



enum agcStages {AGC_RELEASE, AGC_HOLD};
enum agcStages agcStage;

q31_t agcThresh = FtoQ31(0.3f);
q31_t releaseRamp = FtoQ31(0.00001f);
q31_t gainBy16 = FtoQ31(0.1f);

q31_t maxiComp(q31_t input) {
/*
	q31_t gainBy16 = FtoQ31(0.1875f); // gain/16
	q31_t sample = FtoQ31(0.6f);
	q31_t res = 0;
	arm_mult_q31(&sample, &gain, &res, 1);
	//arm_scale_q31(res, 1, -3, res, 1);
	q31_t r = res << 4;
*/


	q31_t inAfterGain;
	arm_mult_q31(&input, &gainBy16, &inAfterGain, 1);
	inAfterGain = inAfterGain << 4;

	q31_t inAbs;
	//arm_abs_q31(&input, &inAbs, 1);
	inAbs = abs(inAfterGain);

	if (inAbs < agcThresh){
		agcStage = AGC_RELEASE;
	}
	else
		agcStage = AGC_HOLD;

	switch (agcStage) {
	case AGC_RELEASE:{
		//if (gainBy16 < (Q31_1-1));
		q31_t newGain = gainBy16 + releaseRamp;
		if (newGain < (Q31_1-1))
			gainBy16 = newGain;
	}
	break;

	case AGC_HOLD:
		//gain += releaseRamp;
	break;
	}

	//q31_t output = 0;
	//arm_copy_q31(inAfterGain, output, 1);

	//gaine = Q31toF(gainBy16)*16.0f;
	gaine = Q31toF(inAbs);

	return inAfterGain;
}




float Falpha = 0.02f;
float FR = 0.7f;
float FAn = 0;

void agcPrasolovFloat(q31_t* source, q31_t* destination, uint16_t blockSize){
	for (uint16_t i=0; i<blockSize; i++){
		float input = Q31toF(source[i]);
		input = input * FAn;

		//float Anew =;
		FAn =  FAn * (1 - (Falpha * fabs(input))) + Falpha * FR;
		destination[i] = FtoQ31(input);
	}
}


q31_t q31Toq28(q31_t in) {return ((q31_t)in << 3);}
q31_t q28Toq31(q31_t in) {return ((q31_t)in >> 3);}

q31_t alpha = FtoQ31(0.02f);
q31_t R = FtoQ31(0.7f);
q31_t An = FtoQ31(0.9f);
q31_t aR = FtoQ31(0.7f * 0.02f);

void agcPrasolovQ(q31_t* source, q31_t* destination, uint16_t blockSize){
	/*
	arm_scale_q31(source, An, 0, destination, blockSize);

	// |in|
	q31_t inAbs[blockSize];
	arm_abs_q31(source, inAbs, blockSize);

	// alpha*|in|
	q31_t alphaIn[blockSize];
	arm_scale_q31(alphaIn, alpha, 0, alphaIn, blockSize);

	// (1 - alpha*|in|)
	q31_t err[blockSize];

	for (uint16_t i=0; i<blockSize; i++)
		err[i] = Q31_1 - alphaIn[i];

	// An * (1 - alpha*|in|)
	q31_t left[blockSize];
	arm_scale_q31(err, An, 0, left, blockSize);

	// (An * (1 - alpha*|in|)) + alpha*R
	q31_t left[blockSize];
	arm_scale_q31(err, An, 0, left, blockSize);
	*/


	// FIMXE: use 4.28?
	for (uint16_t i=0; i<blockSize; i++){
		q31_t sampleIn = source[i];
		q31_t sampleOut;

		arm_scale_q31(&sampleIn, An, 0, &sampleOut, 1);
		destination[i] = sampleOut;

		// |in|
		q31_t inAbs;
		arm_abs_q31(&sampleIn, &inAbs, 1);

		// alpha*|in|
		q31_t alphaIn;
		arm_scale_q31(&inAbs, &alpha, 0, &alphaIn, 1);

		// (1 - alpha*|in|)
		q31_t err;
		err = Q31_1 - alphaIn;

		// An * (1 - alpha*|in|)
		q31_t left;
		arm_scale_q31(&err, &An, 0, &left, 1);

		// (An * (1 - alpha*|in|)) + alpha*R
		q31_t newAn = left + aR;


		An = newAn;
	}

}







