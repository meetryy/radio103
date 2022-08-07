#include "adc.h"
#include "global.h"
#include "main.h"

volatile uint16_t adcDMAdata[ADC_CHANNELS];

adcChannel_t adcCh[ADC_CHANNELS] = {
		{.channel = ADC_CHANNEL_2,	.LPFalpha = 0.01f,	.ADCmax = (1 << ADC_BIT), .ADCperUnit = 0.0f}, // ADC_BUTTONS
		{.channel = ADC_CHANNEL_3,	.LPFalpha = 0.01f,	.ADCmax = (1 << ADC_BIT), .ADCperUnit = 273.0f}, // ADC_VOLTAGE
		{.channel = ADC_CHANNEL_4,	.LPFalpha = 0.01f,	.ADCmax = (1 << ADC_BIT), .ADCperUnit = 273.0f}, // ADC_SWR_F
		{.channel = ADC_CHANNEL_5,	.LPFalpha = 0.01f,	.ADCmax = (1 << ADC_BIT), .ADCperUnit = 273.0f}, // ADC_SWR_R
};

//ADC_HandleTypeDef hadc1;

void adcStart(void){
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adcDMAdata[0], ADC_CHANNELS);
}

//filtered_output[i] = α*raw_input[i] + (1-α)*filtered_output[i-1]

void adcSmoothen(void){
	for (int i=0; i<ADC_CHANNELS; i++){
		/*
		adcCh[i].ADCraw = adcDMAdata[i];
		int16_t delta = adcCh[i].ADCraw - adcCh[i].ADCold;
		//adcCh[i].units = 1.0f / (adcCh[i].ADCperUnit / adcCh[i].ADCsmooth);
		//adcCh[i].ADCsmooth = (uint16_t)(adcCh[i].LPFalpha*(float)adcCh[i].ADCraw + (1.0f-adcCh[i].LPFalpha)*(float)adcCh[i].ADCold);
		adcCh[i].ADCsmooth = adcCh[i].ADCold + ((int16_t)(adcCh[i].LPFalpha * (float)delta));
		adcCh[i].test = (int16_t)(delta * adcCh[i].LPFalpha);
		adcCh[i].ADCold = adcCh[i].ADCsmooth;
		*/

		adcCh[i].ADCraw = adcDMAdata[i];              			//read the sensor value using ADC
		adcCh[i].ADCsmooth = (adcCh[i].LPFalpha*adcCh[i].ADCraw) + ((1-adcCh[i].LPFalpha) * adcCh[i].ADCsmooth);


	}
}

#define ADC_INJ_CHANNELS	(1)
uint32_t	adcActveInjStruct;

#if (ADC_INJ_CHANNELS > 4)
	#error Max umber of injected channels is 4!
#endif

void adcInjInit(void){
	adcActveInjStruct = 0;

	ADC_InjectionConfTypeDef sConfigInjected = {0};
	sConfigInjected.InjectedChannel = adcCh[adcActveInjStruct].channel;// adcActveInjChannels[0];
	sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
	sConfigInjected.InjectedNbrOfConversion = ADC_INJ_CHANNELS;
	sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	sConfigInjected.ExternalTrigInjecConv = ADC_INJECTED_SOFTWARE_START;
	sConfigInjected.AutoInjectedConv = ENABLE;
	sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
	sConfigInjected.InjectedOffset = 0;
	if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
	{
		Error_Handler();
	}
}

inline void adcInjSwitch(uint8_t ADCid){
	  ADC_InjectionConfTypeDef sConfigInjected = {0};

	  sConfigInjected.InjectedChannel = adcCh[ADCid].channel;
	  sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
	  if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  else
		  adcActveInjStruct = ADCid;

}

void adcInjCycleUpdate(void){
	static int currCh;
	adcInjSwitch(currCh);
	adcInjRead();
	currCh++;
	if (currCh > ADC_CHANNELS)
		currCh = 0;

}

inline void adcInjRead(void){
	adcCh[adcActveInjStruct].ADCraw = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
}
