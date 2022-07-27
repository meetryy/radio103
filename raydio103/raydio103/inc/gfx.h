#pragma once
#include <stdint.h>
#include <stdbool.h>

#define LCD_W		(128)
enum fontName 	{FONT_REGULAR, FONT_BIG, FONT_MID, FONT_NR};
extern volatile uint8_t 	GLCD_Buf[1024];

extern char dbgText[7][21];
void gfxInit(void);
void gfxUpdateWhenPossible(void);
