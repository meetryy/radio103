#include "encoder.h"
#include "global.h"

static const int8_t graydecoder [4][4] =
{
	{
		+0,		/* 00 -> 00 stopped				*/
		-1,		/* 00 -> 01 rotate left			*/
		+1,		/* 00 -> 10 rotate right		*/
		+0,		/* 00 -> 11 invalid combination */
	},
	{
		+1,		/* 01 -> 00 rotate right		*/
		+0,		/* 01 -> 01 stopped				*/
		+0,		/* 01 -> 10 invalid combination */
		-1,		/* 01 -> 11 rotate left			*/
	},
	{
		-1,		/* 10 -> 00 rotate left			*/
		+0,		/* 10 -> 01 invalid combination */
		+0,		/* 10 -> 10 stopped				*/
		+1,		/* 10 -> 11 rotate right		*/
	},
	{
		+0,		/* 11 -> 00 invalid combination */
		+1,		/* 11 -> 01 rotate right		*/
		-1,		/* 11 -> 10 rotate left			*/
		+0,		/* 11 -> 11 stopped				*/
	},
};

encoder_t encoder[ENC_NR] = {
		[ENC_UPPER].isReversed = 0,
		[ENC_UPPER].divider = 2,
};

void encInputProcess(int encNum, bool a, bool b){
	encoder_t* e;
	e = &encoder[encNum];

	e->phaseState = (b << 1) | (a);
	if (e->isReversed) 	e->deltaRaw -= graydecoder[e->phaseOldState][e->phaseState];
	else 				e->deltaRaw += graydecoder[e->phaseOldState][e->phaseState];
	e->delta = e->deltaRaw / (1 << e->divider);
	e->phaseOldState = e->phaseState;
}

