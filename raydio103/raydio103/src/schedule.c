#include "agc.h"
#include "dsp.h"
#include "schedule.h"
#include "gfx.h"
#include "si5351.h"
#include "encoder.h"
#include "fft.h"
#include "radio.h"
#include "tools.h"
#include "main.h"
#include "global.h"
#include "metrics.h"
#include "ssd1309.h"
#include "menu.h"

void everythingElse(void);

void schedInit(void){
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_Delay(100);
	debugInit();


	radio.txState = RX;
	dspInit();
	HAL_Delay(10);

	dspStart();
	//gfxDemoDraw();
	ssd1309Init();
	gfxInit();


}

inline void schedMainLoop(void){
	 dspProc();
	 everythingElse();

}


bool elseDone = 0;


/*
 *
	  for (int i=0; i<8; i++){
		  ssd1309Update();
		  HAL_Delay(4);
	  }
	  HAL_Delay(30);
 */

int lastElseTime = 0;
void fillDebugInfo(void){
	snprintf(dbgText[0], 21,  "INP FF1 FF2 FIR AGC");
	snprintf(dbgText[1], 21,  "%03u %03u %03u %03u %03u", 	metrics.metric[0].time - metrics.metric[1].time,
															metrics.metric[1].time - metrics.metric[2].time,
															metrics.metric[2].time - metrics.metric[3].time,
															metrics.metric[3].time - metrics.metric[4].time,
															metrics.metric[4].time - metrics.metric[5].time
															);
	int gfxTicks = (metrics.metric[METRIC_GFX_START].time - metrics.metric[METRIC_GFX_TOTAL].time);
	//int elseTicks = ( - );

	snprintf(dbgText[2], 21,  "OUT DSP  GFX ELSE");
	snprintf(dbgText[3], 21,  "%03u %03u%% %03u %03u%%",	metrics.metric[6].time - metrics.metric[7].time,
															((metrics.metric[0].time - metrics.metric[7].time)*100/DSP_BLOCK_SIZE),
															gfxTicks,
															//lastElseTime*100/DSP_BLOCK_SIZE,
															lastElseTime*100/DSP_BLOCK_SIZE
															);
}

uint32_t nextTime = 2000;

void everythingElse(void){
	//					VVV this makes else start only after DSP is done
	if (!elseDone && dspProcDone){
/*
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
				for (int i=0; i<500; i++)
					__NOP();
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
				*/

			if (HAL_GetTick() >= nextTime){
				menuMove(-1);
				nextTime = HAL_GetTick() + 500;
			}

			setTime(METRIC_ELSE_START);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
				//fillDebugInfo();
				ssd1309PageUpdRoutine();
				gfxUpdateWhenPossible();
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
			setTime(METRIC_ELSE_TOTAL);
			lastElseTime = metrics.metric[METRIC_ELSE_START].time - metrics.metric[METRIC_ELSE_TOTAL].time;


			elseDone = 1;
	}


	// int adc_inj = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
	// encInputProcess(0, encA, encB);
}





