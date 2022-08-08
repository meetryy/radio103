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
	const uint32_t	columnMask;
} fontInfo_t;



const fontInfo_t fontInfo[FONT_NR] = {
		{.dataPtr = &font,	.dataPtrLong = 0,			.W = 5,	.H = 8,		.asciiShift = (32 * 5), .columnMask = 0xFF},	// regular
		{.dataPtr = 0,		.dataPtrLong = &fontBig,	.W = 9,	.H = 16,	.asciiShift = (48 * 9), .columnMask = 0xFFFF},	// big
		{.dataPtr =  0,		.dataPtrLong = &fontMid,	.W = 6,	.H = 11,	.asciiShift = (48 * 6), .columnMask = 0x7FF}	// mid
};

void gfxDrawPoints(int pos){
	if (pos < 6){
		int shiftX = 8 + 10*pos;
		int bitmaps[3] = {0x80, 0xC0, 0x80};

		for (int j=0; j<3; j++){
			GLCD_Buf[j + (LCD_W) + shiftX] 		|= bitmaps[j] << 1;
			GLCD_Buf[j + (LCD_W * 2) + shiftX] 	|= bitmaps[j] >> 7;
		}
	}
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
	uint32_t columnMaskInv = 0x3F;

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
				uint32_t bufIndex = i + (LCD_W * (li + line)) + shiftX;
				GLCD_Buf[bufIndex] &= ~((columnMaskInv << shiftY) >> (8*li));
				GLCD_Buf[bufIndex] |= (shiftedColumn >> (8*li)) & 0xFF;
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
#include "rtc.h"

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

void gfxDrawCategory(int cat, int catNum){

	/*
	 * uint8_t drawPos = 0;
	for (int i = 0; i < (catNum-1); i++){
		GLCD_Buf[drawPos + LCD_W * 7] |= 0x80;
		drawPos += LCD_W / catNum;
	}*/

	if (cat < catNum){

		uint8_t catW = LCD_W / catNum;
		//uint8_t widgetStart = (LCD_W - catW * catNum) / 2;
		uint8_t widgetStart = 2;
		uint8_t catStart = catW * cat + 1;
		uint8_t catEnd = catStart + catW - 2;
		for (int i=0; i<LCD_W; i++){
			if ((i % catW == 0) || ((i > catStart)  &&  (i < catEnd)))
				GLCD_Buf[widgetStart + i + LCD_W * 7] |= 0x80;
		}
	}
}

int sm = 0;
int marker = 0;
int thiscat = 0;
void gfxDemoDraw(void){

		//for (int j = 0; j < 128; j++) {
		//	demoFFTbins[j] = (50*((j>64)&&(j<(64+24))));//rand() % 100;
		//}

		//GLCD_Font_PrintNew(0, 0, "1425030", FONT_BIG);
		//GLCD_Font_PrintNew(69, 6, "5", FONT_MID);

		GLCD_Font_PrintNew(80, 0, "USB", FONT_REGULAR);
		GLCD_Font_PrintNew(98, 0, rtcTimeString, FONT_REGULAR);
		GLCD_Font_PrintNew(80, 10, "2K7", FONT_REGULAR);

		sprintf(txt, "%u%%", dspLoad);
		GLCD_Font_PrintNew(104, 10, txt, FONT_REGULAR);

		GLCD_Font_PrintNew(80, 20, "S+40", FONT_REGULAR);
		GLCD_Font_PrintNew(110, 20, "RXA", FONT_REGULAR);

		GLCD_Font_PrintNew(LOWER_MENU_POS_X0, LOWER_MENU_POS_Y, "MONITR", FONT_REGULAR);
		GLCD_Font_PrintNew(LOWER_MENU_POS_X1, LOWER_MENU_POS_Y, "TONE", FONT_REGULAR);
		//GLCD_Font_PrintNew(92, 54, "TESTNG", FONT_REGULAR);

		sprintf(txt, "%u", freqHz/10);
		GLCD_Font_PrintNew(0, 0, txt, FONT_BIG);

		//GLCD_Font_PrintNew(0, 36, "4 5 6 7 8 9", FONT_MID);

		sprintf(txt, "%u", freqHz  % 10);
		GLCD_Font_PrintNew(70, 5, txt, FONT_MID);
		freqHz++;

		sprintf(txt, "%u", (uint32_t)HAL_GetTick());
		GLCD_Font_PrintNew(LOWER_MENU_POS_X2, LOWER_MENU_POS_Y, txt, FONT_REGULAR);

		uint32_t tim = ((HAL_GetTick() >> 8) & 0x01);
		thiscat = thiscat + tim;
		if (thiscat > 6)
			thiscat = 0;
		gfxDrawCategory(thiscat, 7);

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
	uint32_t columnMaskInv = fontInfo[fontID].columnMask;

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
				uint32_t bufIndex = i + (charCount * fontWspace) + (LCD_W * (li + line)) + shiftX;
				// null old byte where char was
				GLCD_Buf[bufIndex] &= ~((columnMaskInv << shiftY) >> (8*li));
				// draw new char
				GLCD_Buf[bufIndex] |= (shiftedColumn >> (8*li));
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

void gfxInvertPage(int page){
	for (int i = 0; i < LCD_W; i++){
		 GLCD_Buf[i + (LCD_W * page)] = ~ GLCD_Buf[i +  (LCD_W * page)];
	}
}

#include "menu.h"
// menu lines being filled outside
void gfxDrawMenu(void){
	//snprintf(MENU_MAX_LINE_LEN, menuLinesTxt[0], "FUCK");
	menuFillLines(0);
	GLCD_Font_PrintNew(0, 0, menuHeader, FONT_REGULAR);
	gfxInvertPage(0);

	for (int i=0; i<MENU_LINES; i++){
		//snprintf(dbgText[i], 20, "%u", HAL_GetTick());
		GLCD_Font_PrintNew(0, 9*i + 9, menuLinesTxt[i], FONT_REGULAR);
	}

	GLCD_Font_PrintNew(LOWER_MENU_POS_X0, LOWER_MENU_POS_Y, "SAVE", FONT_REGULAR);
	GLCD_Font_PrintNew(LOWER_MENU_POS_X2, LOWER_MENU_POS_Y, "EXIT", FONT_REGULAR);

}

void gfxRLEtoFramebuffer(uint8_t* RLEdata){
	uint32_t blockCounter = 0;
	uint32_t maxBlocks = sizeof(splashRLEData)/sizeof(splashRLEData[0])/2;
	uint32_t bufPtr = 0;
	for (int block = 0; block < maxBlocks; block++){
		blockCounter++;
		uint8_t byteNum = splashRLEData[block * 2];
		uint8_t byteVal = splashRLEData[block * 2 + 1];
		for (int byteCnt = 0; byteCnt < byteNum; byteCnt++){
			GLCD_Buf[bufPtr] = byteVal;
			bufPtr++;
		}
	}
}


void gfxInit(void){
	gfxClearBuffer();
}

void gfxClearBuffer(void){
	//memset((uint32_t*)GLCD_Buf, 0, sizeof(GLCD_Buf)/4);
	memset(GLCD_Buf, 0, sizeof(GLCD_Buf));
}

void gfxCanaryBuffer(){
	//memset(GLCD_Buf, 0x55, sizeof(GLCD_Buf));
	for (int i=0; i< (128*8);i+=2){
		GLCD_Buf[i] = 0x55;
		GLCD_Buf[i+1] = 0xAA;
	}
}

void gfxUpdateWhenPossible(void){
		gfxCanaryBuffer();
		//gfxClearBuffer();
		//gfxRLEtoFramebuffer(0);

		//gfxDrawMenu();
		//gfxDemoDraw();
		//gfxDrawDebugInfo();
}

// TODO: gfx add volume slider
// TODO: gfx generic progressbar
// TODO: gfx catgory indicator




