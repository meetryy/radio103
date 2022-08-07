#pragma once
#include <stdint.h>
#include <stdbool.h>

#define LCD_W		(128)

#define LOWER_MENU_POS_X0 	(0)
#define LOWER_MENU_POS_X1 	(42)
#define LOWER_MENU_POS_X2 	(86)
#define LOWER_MENU_POS_Y 	(54)

enum fontName 	{FONT_REGULAR, FONT_BIG, FONT_MID, FONT_NR};
extern volatile uint8_t 	GLCD_Buf[1024];



extern char dbgText[7][21];
void gfxInit(void);
void gfxUpdateWhenPossible(void);
void gfxRLEtoFramebuffer(uint8_t* RLEdata);
