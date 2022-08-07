#include "menu.h"
#include "global.h"
#include <stdio.h>



menuItem_t menuItems[MENU_NR] = {
		{.name =  "FLT DUMMY          \0", 	.dataType = TYPE_FLOAT, .data.f = 25000000,	.min = 0,		.max = 100000000, 	.step = 1, 		.funcOnUpdate.f = 0,	.dataPtr.f = 0},	// MENU_FLOAT_DUMMY
		{.name =  "BTN BEEP           \0", 	.dataType = TYPE_BOOL, 	.data.b = 1,		.min = 0,		.max = 1, 			.step = 1, 		.funcOnUpdate.b = 0,	.dataPtr.b = 0},	// MENU_BEEP_ON
		{.name =  "BEEP TONE          \0", 	.dataType = TYPE_INT, 	.data.i = 500,		.min = 200,		.max = 2000, 		.step = 100, 	.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_BEEP_TONE
		{.name =  "BEEP VOL           \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_BEEP_VOL
		{.name =  "ENCODER REV        \0", 	.dataType = TYPE_BOOL, 	.data.b = 0,		.min = 0,		.max = 1, 			.step = 1, 		.funcOnUpdate.b = 0,	.dataPtr.b = 0},	// MENU_ENC_DIR
		{.name =  "ENCODER DIV        \0", 	.dataType = TYPE_INT, 	.data.i = 0,		.min = 1,		.max = 8, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_ENC_DIV
		{.name =  "CW SPEED           \0", 	.dataType = TYPE_INT, 	.data.i = 30,		.min = 1,		.max = 60, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_CW_SPEED
		{.name =  "CW TONE            \0", 	.dataType = TYPE_INT, 	.data.i = 700,		.min = 100,		.max = 2000, 		.step = 100, 	.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_CW_TONE
		{.name =  "CW AUTO            \0", 	.dataType = TYPE_BOOL, 	.data.b = 1,		.min = 0,		.max = 1, 			.step = 1, 		.funcOnUpdate.b = 0,	.dataPtr.b = 0},	// MENU_CW_MODE
		{.name =  "CW SWAP PAD        \0", 	.dataType = TYPE_BOOL, 	.data.b = 1,		.min = 0,		.max = 1, 			.step = 1,		.funcOnUpdate.b = 0,	.dataPtr.b = 0},	// MENU_CW_SWAP
		{.name =  "CW WEIGHT          \0", 	.dataType = TYPE_FLOAT, .data.f = 3.1f,		.min = 2.0f,	.max = 4.0f, 		.step = 0.1f, 	.funcOnUpdate.f = 0,	.dataPtr.f = 0},	// MENU_CW_WEIGHT

		{.name =  "DISP BRIGHT        \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "DISP ROTATE        \0", 	.dataType = TYPE_BOOL, 	.data.b = 0,		.min = 0,		.max = 1, 			.step = 1, 		.funcOnUpdate.b = 0,	.dataPtr.b = 0},	// DISP_ROTATE
		{.name =  "DISP INVERSE       \0", 	.dataType = TYPE_BOOL, 	.data.b = 0,		.min = 0,		.max = 1, 			.step = 1, 		.funcOnUpdate.b = 0,	.dataPtr.b = 0},	// DISP_INVERSE
		{.name =  "DSP NR LVL         \0", 	.dataType = TYPE_INT, 	.data.i = 3,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DSP_NR_LVL
		{.name =  "DSP MIC GAIN       \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DSP_MIC_GAIN
		{.name =  "DSP TX LVL         \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DSP_TX_LVL

		{.name =  "SHOW SPLASH        \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC BTN 0 CAL      \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC BTN 1 CAL      \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC BTN 2 CAL      \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC BTN 3 CAL      \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC BTN 4 CAL      \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC BTN 5 CAL      \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC SWR F CAL      \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC SWR R CAL      \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC VIN CAL        \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC S9 CAL         \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "ADC S+20 CAL       \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "MENU ALT FONT      \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "RTC HOURS          \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS
		{.name =  "RTC MINUTES        \0", 	.dataType = TYPE_INT, 	.data.i = 5,		.min = 0,		.max = 10, 			.step = 1, 		.funcOnUpdate.i = 0,	.dataPtr.i = 0},	// MENU_DISP_BRIGHTNESS

		{.name =  "CLK XTAL           \0", 	.dataType = TYPE_FLOAT, .data.f = 25000000,	.min = 0,		.max = 100000000, 	.step = 1, 		.funcOnUpdate.f = 0,	.dataPtr.f = 0},	// MENU_CLK_XTAL


};


// MENU_SHOW_SPLASH, MENU_BTN0_ADC, /*...*/ MENU_BTN5_ADC,
		// MENU_SWR_F_CALIB, MENU_SWR_R_CALIB, MENU_VIN_CALIB,
		// MENU_SMETER_9_CALIB, MENU_SMETER_20_CLIB,
		// MENU_FONT,
		// MENU_RTC_H, MENU_RTC_M,
		//
uint8_t menuCurrentPos = 0;
uint8_t menuPoints[2] = {0, (MENU_LINES-1)};

/*
void menuCheck(void){
	for (int i = 0; i < MENU_NR; i++){
		if (menuItems[i].dataType == TYPE_BOOL)
	}
}
*/

enum {BEGIN, END};
void menuMove (int delta){
	int newPos = menuCurrentPos + delta;
	if (newPos < 0) 				menuCurrentPos = newPos + MENU_NR;
	else if (newPos > (MENU_NR-1)) 	menuCurrentPos = newPos % MENU_NR;
	else menuCurrentPos = newPos;

	if (menuCurrentPos > menuPoints[END]) {
		menuPoints[END] = menuCurrentPos;
		menuPoints[BEGIN] = menuPoints[END] - (MENU_LINES-1);
	}
	if (menuCurrentPos < menuPoints[BEGIN]) {
		menuPoints[BEGIN] = menuCurrentPos;
		menuPoints[END] = menuPoints[BEGIN] + (MENU_LINES-1);
	}
	{}
}

char menuLinesTxt[MENU_LINES][MENU_MAX_LINE_LEN];
char menuHeader[MENU_MAX_LINE_LEN];

const char txtOnOff[2][3] = {"OFF"," ON"};

void menuFillLines(uint8_t pos){
	snprintf(menuHeader, MENU_MAX_LINE_LEN, "SETTINGS      %02u / %02u\0", menuCurrentPos+1, MENU_NR);
	//snprintf(menuHeader, MENU_MAX_LINE_LEN, "%02u / %02u / %02u\0", menuPoints[BEGIN], menuCurrentPos, menuPoints[END]);
	for (int line = 0; line < MENU_LINES; line++){
		int thisLine = line + menuPoints[BEGIN];
		snprintf(menuLinesTxt[line], MENU_MAX_LINE_LEN, "                     \0");
		if (line > menuPoints[END]) break;

		//snprintf(MENU_MAX_LINE_LEN, (char*)menuLinesTxt[line], "%s", "GO!");
		strncpy(menuLinesTxt[line]+1, menuItems[thisLine].name, MENU_MAX_LINE_LEN);
		if (thisLine == menuCurrentPos) 	menuLinesTxt[line][0] = '>';
		else 							menuLinesTxt[line][0] = ' ';

		const int bufLen = 12;
		char txtBuf[bufLen];
		switch (menuItems[thisLine].dataType){
			case TYPE_BOOL: {
				if (menuItems[thisLine].data.b == 0 | menuItems[thisLine].data.b == 1)
					strncpy(menuLinesTxt[line] + MENU_MAX_LINE_LEN - 4, txtOnOff[menuItems[thisLine].data.b], 3);
				break;
			}

			case TYPE_INT: {
					snprintf(txtBuf, bufLen, "%u\0", menuItems[thisLine].data.i);
					strncpy(menuLinesTxt[line] + MENU_MAX_LINE_LEN - strlen(txtBuf) - 1, txtBuf, strlen(txtBuf));
				break;
			}

			case TYPE_FLOAT: {
				snprintf(txtBuf, bufLen, "%.1f\0", menuItems[thisLine].data.f);
				strncpy(menuLinesTxt[line] + MENU_MAX_LINE_LEN - strlen(txtBuf) - 1, txtBuf, strlen(txtBuf));
				break;
			}
			default: {
				snprintf(txtBuf, bufLen, "(err)\0");
				strncpy(menuLinesTxt[line] + MENU_MAX_LINE_LEN - strlen(txtBuf) - 1, txtBuf, strlen(txtBuf));
				break;
			}
		}
	}
}

void menuChangeData (int delta){
	menuItem_t* thisItem = &menuItems[menuCurrentPos];
	switch (thisItem->dataType){
		case TYPE_BOOL:{
				bool newData = !thisItem->data.b;
				thisItem->data.b = newData;
				if (thisItem->dataPtr.b != 0)
					*thisItem->dataPtr.b = newData;
				if (thisItem->funcOnUpdate.b != 0)
					thisItem->funcOnUpdate.b(newData);
					//*thisItem->dataPtr.b = newData;
			break;
		}

		case TYPE_INT:{
				int newData = thisItem->data.i + delta * (int)thisItem->step;
				if (newData > (int)thisItem->max)
					newData =  (int)thisItem->max;
				if (newData < (int)thisItem->min)
					newData =  (int)thisItem->min;
				thisItem->data.i = newData;
				if (thisItem->dataPtr.i != 0)
					*thisItem->dataPtr.i = newData;
				if (thisItem->funcOnUpdate.i != 0)
					thisItem->funcOnUpdate.i(newData);
			break;
		}

		case TYPE_FLOAT:{
				float newData = thisItem->data.f + delta * (float)thisItem->step;
				if (newData > (float)thisItem->max)
					newData =  (float)thisItem->max;
				if (newData < (float)thisItem->min)
					newData =  (float)thisItem->min;
				thisItem->data.f = newData;
				if (thisItem->dataPtr.f != 0)
					*thisItem->dataPtr.f = newData;
				if (thisItem->funcOnUpdate.f != 0)
					thisItem->funcOnUpdate.f(newData);
			break;
		}
		default: {

			break;
		}
	}
}
