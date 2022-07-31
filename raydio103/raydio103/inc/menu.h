#pragma once

#include "menu.h"
#include <stdint.h>
#include <stdbool.h>

#define MENU_MAX_LINE_LEN	(22)
#define MENU_LINES			(5)

enum menuDataType {TYPE_BOOL, TYPE_INT, TYPE_FLOAT};
enum menuItems {MENU_FLOAT_DUMMY,
				MENU_BEEP_ON, MENU_BEEP_TONE, MENU_BEEP_VOL,
				MENU_ENC_DIR, MENU_ENC_DIV,
				MENU_CW_SPEED, MENU_CW_TONE, MENU_CW_MODE, MENU_CW_SWAP, MENU_CW_WEIGHT,
				MENU_DISP_BRIGHTNESS, DISP_ROTATE, DISP_INVERSE,
				MENU_DSP_NR_LVL, MENU_DSP_MIC_GAIN, MENU_DSP_TX_LVL,
				// MENU_SHOW_SPLASH, MENU_BTN0_ADC, /*...*/ MENU_BTN5_ADC,
				// MENU_SWR_F_CALIB, MENU_SWR_R_CALIB, MENU_VIN_CALIB,
				// MENU_SMETER_9_CALIB, MENU_SMETER_20_CLIB,
				// MENU_FONT,
				// MENU_RTC_H, MENU_RTC_M,
				//
				MENU_CLK_XTAL,
				MENU_NR};

typedef struct {
	union {
	   int i;
	   float f;
	   bool b;
	} data;
	uint8_t dataType;

	float min;
	float max;
	float step;

	char name[20];
	void* funcOnUpdate;

} menuItem_t;

extern menuItem_t menuItems[MENU_NR];
extern char menuLinesTxt[MENU_LINES][MENU_MAX_LINE_LEN];
extern char menuHeader[MENU_MAX_LINE_LEN];

void menuPreload(void);
void menuOpen(void);
void menuFillLines(uint8_t pos);
void menuMove (int delta);
void menuChange (int delta);
void menuClose(void);
