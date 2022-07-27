#include "gfx.h"
#include "global.h"
#include "main.h"
#include "font.h"
#include "ssd1309.h"

uint8_t startRow, startCol, endRow, endCol; // coordinates of the dirty rectangle
uint8_t numRows = 64;
uint8_t numCols = 128;
volatile uint8_t GLCD_Buf[1024];

// Set Pixel
void SetPixel(uint8_t x, uint8_t y)
{
  if (y < numRows && x < numCols)
  {
		GLCD_Buf[(x)+((y/8)*128)]|=0x01<<y%8;

    // Change the dirty rectangle to account for a pixel being dirty (we assume it was changed)
    if (startRow > y) { startRow = y; }
    if (endRow <= y)  { endRow = y + 1; }
    if (startCol > x) { startCol = x; }
    if (endCol <= x)  { endCol = x + 1; }
  }
}

// Clear Pixel
void ClearPixel(uint8_t x, uint8_t y)
{
  if (y < numRows && x < numCols)
  {
		GLCD_Buf[(x)+((y/8)*128)]&=(~(0x01<<y%8));

    // Change the dirty rectangle to account for a pixel being dirty (we assume it was changed)
    if (startRow > y) { startRow = y; }
    if (endRow <= y)  { endRow = y + 1; }
    if (startCol > x) { startCol = x; }
    if (endCol <= x)  { endCol = x + 1; }
  }
}


typedef struct {
	const int W;
	const int H;
	const int asciiShift;
	const uint8_t* dataPtr;
	const uint16_t* dataPtrLong;
} fontInfo_t;

const fontInfo_t fontInfo[FONT_NR] = {
		// regular
		{
		.dataPtr = &font,
		.dataPtrLong = 0,
		.W = 5,
		.H = 8,
		.asciiShift = (32 * 5)
		},

		// big
		{
		.dataPtr = 0,
		.dataPtrLong = &fontBig,
		.W = 9,
		.H = 16,
		.asciiShift = (48 * 9)
		},

		// mid
		{
		.dataPtr =  0,
		.dataPtrLong = &fontMid,
		.W = 6,
		.H = 11,
		.asciiShift = (48 * 6)
		}

};

void gfxDrawPoints(int pos){
	//for (int i=0; i<6; i++){
		int shiftX = 8 + 10*pos;
		int bitmaps[3] = {0x80, 0xC0, 0x80};

		for (int j=0; j<3; j++){
			GLCD_Buf[j + (LCD_W) + shiftX] 		|= bitmaps[j] << 1;
			GLCD_Buf[j + (LCD_W * 2) + shiftX] 	|= bitmaps[j] >> 7;
		}

	//}

}

const uint8_t smBitmap[] = {

	0x1E, // start & end
	0x3F, // all black
	0x33, // notch
	0x2D, // bar end
	0x21, // empty
	/*
	0x3E, // start & end
	0x7F, // all black
	0x77, // notch
	0x5D, // bar end
	0x41, // empty
*/
};

enum {SM_STARTEND, SM_FULL, SM_NOTCH, SM_TIP, SM_EMPTY};

void gfxDrawSmeter(int percent){
	uint8_t x = 1;
	uint8_t y = 22;
	uint8_t meterH = 6;
	uint8_t meterW = 76;
	uint8_t barLen = meterW * percent / 100;

	int shiftX = x;
	int charCount = 0;


	int line = y / 8;
	int shiftY = y % 8;
	uint32_t shiftedColumn = 0;
	int takesLines = 2;

	uint8_t bitmap = 0;
	for(int i = 0; i < meterW; i++){
			if (i == (meterW - 1)) bitmap = smBitmap[SM_STARTEND];
			else if (i == 0) bitmap = smBitmap[SM_STARTEND];
			else if (i < barLen){
				if (!(i % 8)) 	bitmap = smBitmap[SM_NOTCH];
				else			bitmap = smBitmap[SM_FULL];
			}
			else if (i == barLen) bitmap = smBitmap[SM_TIP];
			else bitmap = smBitmap[SM_EMPTY];

			shiftedColumn = bitmap << shiftY;

			for (int li = 0; li < takesLines; li++){
				 GLCD_Buf[i + (LCD_W * (li + line)) + shiftX] |= (shiftedColumn >> (8*li)) & 0xFF;
			}
	}


}

#include <stdlib.h>

char txt[16];
int 	freqHz = 14250000;

int 	benchTimes = 1;
int		benchStart = 0;
float 	benchTimeUs = 0;

int		demoFFTbins[128];

#include "dsp.h"

void gfxDrawFFT(void){
	uint8_t x = 0;
	uint8_t y = 30;
	uint8_t meterH = 24;
	uint8_t meterW = 128;

	int shiftX = x;

	int line = y / 8;
	int shiftY = y % 8;

	uint32_t shiftedColumn = 0;
	int takesLines = 4; // FIXME: too much? add correct calculation

	uint32_t bitmap = 0;
	for(int i = 0; i < meterW; i++){
			// i is bin index
			int binH = magnitudes[i] * meterH / 100;
			bitmap = 0x00FFFFFF << (meterH - binH);
			shiftedColumn = bitmap << shiftY;

			for (int li = 0; li < takesLines; li++){
				 GLCD_Buf[i + (LCD_W * (li + line)) + shiftX] |= (shiftedColumn >> (8 * li)) & 0xFF;
			}
	}
}

const int markerY = 17;
void gfxDrawMarker(int pos){
	int markerW = fontInfo[FONT_BIG].W - 4;
	int shiftY = 1;
	int shiftX = pos * 10;
	int line = 2;
	for(int i = 0; i < markerW; i++){
		int bitmap = 0x03;
		if ((i==0) | (i==markerW-1)) bitmap = 0x01;

		int shiftedColumn = bitmap << shiftY;
		GLCD_Buf[2+i + (LCD_W * (line)) + shiftX] |= (shiftedColumn) & 0xFF;
		//GLCD_Buf[i + (LCD_W * (line)) + shiftX] |= (shiftedColumn) & 0xFF;
	}
}

int sm = 0;
int marker = 0;
void gfxDemoDraw(void){

		//for (int j = 0; j < 128; j++) {
		//	demoFFTbins[j] = (50*((j>64)&&(j<(64+24))));//rand() % 100;
		//}

		//GLCD_Font_PrintNew(0, 0, "1425030", FONT_BIG);
		//GLCD_Font_PrintNew(69, 6, "5", FONT_MID);

		GLCD_Font_PrintNew(80, 0, "USB", FONT_REGULAR);
		GLCD_Font_PrintNew(98, 0, "12:35", FONT_REGULAR);
		GLCD_Font_PrintNew(80, 10, "2K7", FONT_REGULAR);

		sprintf(txt, "%u%%", dspLoad);
		GLCD_Font_PrintNew(104, 10, txt, FONT_REGULAR);

		GLCD_Font_PrintNew(80, 20, "S+40", FONT_REGULAR);
		GLCD_Font_PrintNew(110, 20, "RXA", FONT_REGULAR);

		GLCD_Font_PrintNew(1, 55, "MONITR", FONT_REGULAR);
		GLCD_Font_PrintNew(46, 55, "TONE", FONT_REGULAR);
		GLCD_Font_PrintNew(92, 55, "TESTNG", FONT_REGULAR);

		sprintf(txt, "%u", freqHz/10);
		GLCD_Font_PrintNew(0, 0, txt, FONT_BIG);

		//GLCD_Font_PrintNew(0, 36, "4 5 6 7 8 9", FONT_MID);

		sprintf(txt, "%u", freqHz  % 10);
		GLCD_Font_PrintNew(70, 5, txt, FONT_MID);
		freqHz++;


		gfxDrawPoints(1);
		gfxDrawPoints(4);
		gfxDrawFFT();

		gfxDrawSmeter(sm);
		gfxDrawMarker(4);

		sm = rand()%100;

}


void gfxDemo(void){
	while(1){

		for (int j = 0; j < 128; j++) {
			demoFFTbins[j] = rand() % 100;
		}

		benchStart = HAL_GetTick();
		for (int i=0; i < benchTimes; i++)
			gfxDemoDraw();

		int benchMs = HAL_GetTick() - benchStart;
		benchTimeUs = 1000.0f * (float)benchMs / (float)benchTimes;
		sprintf(txt, "%u US", (int)benchTimeUs);
		GLCD_Font_PrintNew(0, 33, txt, FONT_REGULAR);

		//ST7920_Update();
		//HAL_Delay(100);
	}
}

void GLCD_Font_PrintNew(uint8_t x, uint8_t y, char *String, int fontID){
	int shiftX = x;
	int charCount = 0;

	int fontWspace = fontInfo[fontID].W + 1;

	int line = y / 8;
	int shiftY = y % 8;
	int takesLines = fontInfo[fontID].H / 8 + (shiftY > 0);
	uint32_t shiftedColumn = 0;

	int i;
	while(*String)
	{
		uint32_t dataPointer = (*String) * fontInfo[fontID].W - fontInfo[fontID].asciiShift;
		for(i=0; i < fontInfo[fontID].W; i++){
			if (fontInfo[fontID].dataPtrLong == 0)
				shiftedColumn = fontInfo[fontID].dataPtr[dataPointer + i] << shiftY;
			else
				shiftedColumn = fontInfo[fontID].dataPtrLong[dataPointer + i] << shiftY;

				for (int li = 0; li < takesLines; li++){
					 GLCD_Buf[i + (charCount * fontWspace) + (LCD_W * (li + line)) + shiftX] |= (shiftedColumn >> (8*li)) & 0xFF;
				}
		}

		String++;
		charCount++;

	}
}

char dbgText[7][21];
void gfxDrawDebugInfo(void){
	for (int i=0; i<7; i++){
		//snprintf(dbgText[i], 20, "%u", HAL_GetTick());
		GLCD_Font_PrintNew(0, 9*i, dbgText[i], FONT_REGULAR);
	}

}

void gfxInit(void){
	gfxClearBuffer();
}

void gfxClearBuffer(void){
	memset(GLCD_Buf, 0, sizeof(GLCD_Buf));
}

#include "metrics.h"

void gfxUpdateWhenPossible(void){
	if (lcdUpdateAllowed){
		setTime(METRIC_GFX_START);
		gfxClearBuffer();

		gfxDemoDraw();

		//gfxDrawDebugInfo();
		lcdUpdateAllowed = 0;
		setTime(METRIC_GFX_TOTAL);
	}
}