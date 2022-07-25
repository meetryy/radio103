/*
 * ST7920_SERIAL.c
 *
 *  Created on: 07-Jun-2019
 *      Author: poe
 *
 *	Edit By	:		Liyanboy74
 *	Date	:	21-July-2020
 */

#include "main.h"
#include "st7920.h"
#include "font.h"

#define CS_PIN 	GPIO_PIN_12
#define CS_PORT GPIOA

uint8_t startRow, startCol, endRow, endCol; // coordinates of the dirty rectangle
uint8_t numRows = 64;
uint8_t numCols = 128;
uint8_t Graphic_Check = 0;

//GLCD Buf
uint8_t GLCD_Buf[1024];

void delay_us(uint32_t Time_us)
{
	//uint16_t i;
	while(Time_us--);//for(i=0;i<0xf;i++);
}

// A replacement for SPI_TRANSMIT

//SPI_HandleTypeDef hspi1;

void ST7920_SendCmd (uint8_t cmd)
{

	uint8_t data[3] = {0};
	data[0] = 0xf8 + (0<<1);
	data[1] = cmd & 0xf0;
	data[2] = (cmd << 4) & 0xf0;


	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);  // PUll the CS high
		HAL_SPI_Transmit(&hspi1, (uint8_t*)&data, 3, 2);
		//delay_us(50);
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);  // PUll the CS LOW

}

void ST7920_SendData (uint8_t data)
{

	uint8_t dat[3] = {0};
	dat[0] = 0xf8+(1<<1);
	dat[1] = data&0xf0;
	dat[2] = (data<<4)&0xf0;


	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);  // PUll the CS high
		HAL_SPI_Transmit(&hspi1, (uint8_t*)&dat, 3, 2);
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);  // PUll the CS LOW
}

// switch to graphic mode or normal mode::: enable = 1 -> graphic mode enable = 0 -> normal mode

void ST7920_GraphicMode (int enable)   // 1-enable, 0-disable
{
	if (enable == 1)
	{
		ST7920_SendCmd(0x30);  // 8 bit mode
		HAL_Delay (1);
		ST7920_SendCmd(0x34);  // switch to Extended instructions
		HAL_Delay (1);
		ST7920_SendCmd(0x36);  // enable graphics
		HAL_Delay (1);
		Graphic_Check = 1;  // update the variable
	}

	else if (enable == 0)
	{
		ST7920_SendCmd(0x30);  // 8 bit mode
		HAL_Delay (1);
		Graphic_Check = 0;  // update the variable
	}
}

typedef int byte;

void gotoXY(byte x, byte y)
{
  if(y>=32 && y<64) { y-=32; x+=8; } else
  if(y>=64 && y<64+32) { y-=32; x+=0; } else
  if(y>=64+32 && y<64+64) { y-=64; x+=8; }
  ST7920_SendCmd(0x80 | y); // 6-bit (0..63)
  ST7920_SendCmd(0x80 | x); // 4-bit (0..15)
}

void ST7920_DrawBitmap(const unsigned char* graphic, bool half)
{

	uint8_t x, y;

	uint16_t Index=0;
	uint8_t Temp,Db;
	//int Ystart = 0+half*64;
	//int Yend = 64+half*64;



	for(y=0;y<60;y++)
	{
		for(x=0;x<8;x++)
		{
			if(y<32)//Up
			{
				ST7920_SendCmd(0x80 | y);										//Y(0-31)
				ST7920_SendCmd(0x80 | x);										//X(0-8)
			}
			else
			{
				ST7920_SendCmd(0x80 | y-32);//Y(0-31)
				ST7920_SendCmd(0x88 | x);//X(0-8)
			}

			Index=((y/8)*128)+(x*16);
			Db=y%8;

			Temp=	(((graphic[Index+0]>>Db)&0x01)<<7)|
						(((graphic[Index+1]>>Db)&0x01)<<6)|
						(((graphic[Index+2]>>Db)&0x01)<<5)|
						(((graphic[Index+3]>>Db)&0x01)<<4)|
						(((graphic[Index+4]>>Db)&0x01)<<3)|
						(((graphic[Index+5]>>Db)&0x01)<<2)|
						(((graphic[Index+6]>>Db)&0x01)<<1)|
						(((graphic[Index+7]>>Db)&0x01)<<0);
			ST7920_SendData(Temp);

			Temp=	(((graphic[Index+8]>>Db)&0x01)<<7)|
						(((graphic[Index+9]>>Db)&0x01)<<6)|
						(((graphic[Index+10]>>Db)&0x01)<<5)|
						(((graphic[Index+11]>>Db)&0x01)<<4)|
						(((graphic[Index+12]>>Db)&0x01)<<3)|
						(((graphic[Index+13]>>Db)&0x01)<<2)|
						(((graphic[Index+14]>>Db)&0x01)<<1)|
						(((graphic[Index+15]>>Db)&0x01)<<0);

			ST7920_SendData(Temp);
		}

	}

}

//Clear GLCD Buf
void GLCD_Buf_Clear(void)
{
	uint16_t i;
	for(i=0;i<1024;i++)GLCD_Buf[i]=0;
}

#include <stdbool.h>
bool lastHalf;
// Update the display with the selected graphics
void ST7920_Update(void)
{
	ST7920_DrawBitmap(GLCD_Buf, 0);
}



void ST7920_Switch(bool i)
{
	ST7920_SendCmd(0x03);
	ST7920_SendCmd(0x40 + 32 * i);
}

void ST7920_Clear()
{
	if (Graphic_Check == 1)  // if the graphic mode is set
	{
		uint8_t x, y;
		for(y = 0; y < 64; y++)
		{
			if(y < 32)
			{
				ST7920_SendCmd(0x80 | y);
				ST7920_SendCmd(0x80);
			}
			else
			{
				ST7920_SendCmd(0x80 | (y-32));
				ST7920_SendCmd(0x88);
			}
			for(x = 0; x < 8; x++)
			{
				ST7920_SendData(0);
				ST7920_SendData(0);
			}
		}
		GLCD_Buf_Clear();
	}

	else
	{
		ST7920_SendCmd(0x01);   // clear the display using command
		HAL_Delay(2); // delay >1.6 ms
	}
}

#define LCD_CLS         0x01
#define LCD_HOME        0x02
#define LCD_ADDRINC     0x06
#define LCD_DISPLAYON   0x0C
#define LCD_DISPLAYOFF  0x08
#define LCD_CURSORON    0x0E
#define LCD_CURSORBLINK 0x0F
#define LCD_BASIC       0x30
#define LCD_EXTEND      0x34
#define LCD_GFXMODE     0x36
#define LCD_TXTMODE     0x34
#define LCD_STANDBY     0x01
#define LCD_SCROLL      0x03
#define LCD_SCROLLADDR  0x40
#define LCD_ADDR        0x80
#define LCD_LINE0       0x80
#define LCD_LINE1       0x90
#define LCD_LINE2       0x88
#define LCD_LINE3       0x98

void ST7920_Init (void)
{

	HAL_Delay(50);   //wait for >40 ms

	ST7920_SendCmd(LCD_BASIC);  // 8bit mode
	HAL_Delay(1);

	ST7920_SendCmd(LCD_BASIC);  // 8bit mode
	HAL_Delay(1);

	ST7920_SendCmd(LCD_CLS);  // D=0, C=0, B=0
	HAL_Delay(1);

	ST7920_SendCmd(LCD_ADDRINC);  // clear screen
	HAL_Delay(1);


	ST7920_SendCmd(LCD_DISPLAYON);  // cursor increment right no shift
	//ST7920_SendCmd(0x07);
	HAL_Delay(1);  // 1ms delay

	ST7920_SendCmd(LCD_EXTEND);  // D=1, C=0, B=0
    HAL_Delay(1);  // 1ms delay

    ST7920_SendCmd(LCD_GFXMODE);  // D=1, C=0, B=0
    HAL_Delay(1);  // 1ms dela


}

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
		.W = 5,
		.H = 11,
		.asciiShift = (48 * 5)
		}

};

void gfxDrawPoints(void){
	SetPixel(20, 16);
	SetPixel(21, 15);
	SetPixel(21, 16);
	SetPixel(22, 16);
}

const uint8_t smBitmap[] = {
	0x1E, // start & end
	0x3F, // all black
	0x33, // notch
	0x2D, // bar end
	0x21, // empty
};

enum {SM_STARTEND, SM_FULL, SM_NOTCH, SM_TIP, SM_EMPTY};

void gfxDrawSmeter(int percent){

	uint8_t x = 1;
	uint8_t y = 23;
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
				 GLCD_Buf[i + (LCD_W * (li + line)) + shiftX] = (shiftedColumn >> (8*li)) & 0xFF;
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

void gfxDrawFFT(void){
	uint8_t x = 0;
	uint8_t y = 25;
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
			int binH = demoFFTbins[i] * meterH / 100;
			bitmap = 0xFFFFFFFF << (meterH - binH);
			shiftedColumn = bitmap << shiftY;

			for (int li = 0; li < takesLines; li++){
				 GLCD_Buf[i + (LCD_W * (li + line)) + shiftX] = (shiftedColumn >> (8 * li)) & 0xFF;
			}
	}
}

int sm = 0;
void gfxDemoDraw(void){

		for (int j = 0; j < 128; j++) {
			demoFFTbins[j] = (50*((j>64)&&(j<(64+24))));//rand() % 100;
		}

		//GLCD_Font_PrintNew(0, 0, "1425030", FONT_BIG);
		GLCD_Font_PrintNew(69, 6, "5", FONT_MID);

		GLCD_Font_PrintNew(80, 0, "USB", FONT_REGULAR);
		GLCD_Font_PrintNew(98, 0, "12:35", FONT_REGULAR);
		GLCD_Font_PrintNew(80, 10, "2K7", FONT_REGULAR);
		GLCD_Font_PrintNew(104, 10, "10.7", FONT_REGULAR);
		GLCD_Font_PrintNew(80, 20, "S+40", FONT_REGULAR);
		GLCD_Font_PrintNew(104, 20, "RX-A", FONT_REGULAR);

		GLCD_Font_PrintNew(1, 55, "MONITR", FONT_REGULAR);
		GLCD_Font_PrintNew(46, 55, "TONE", FONT_REGULAR);
		GLCD_Font_PrintNew(92, 55, "TESTNG", FONT_REGULAR);

		sprintf(txt, "%u", freqHz/10);
		GLCD_Font_PrintNew(0, 0, txt, FONT_BIG);

		sprintf(txt, "%u", freqHz  % 10);
		GLCD_Font_PrintNew(70, 6, txt, FONT_MID);
		freqHz++;

		gfxDrawFFT();

		gfxDrawSmeter(sm);

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

		ST7920_Update();
		HAL_Delay(100);
	}
}

void GLCD_Font_PrintNew(uint8_t x, uint8_t y, char *String, int fontID)
{
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
					 GLCD_Buf[i + (charCount * fontWspace) + (LCD_W * (li + line)) + shiftX] = (shiftedColumn >> (8*li)) & 0xFF;
				}
		}

		String++;
		charCount++;

	}
}

void GLCD_Font_Print(uint8_t x,uint8_t y,char * String)
{
	int shiftX = 0;
	int shiftY = 1;

	int fontW = 5;
	int fontWspace = fontW + 1;
	int asciiShift = 32 * fontW;

	int i;
	while(*String)
	{
		for(i=0; i<fontW; i++)
			GLCD_Buf[i + (x * fontWspace) +  + (y * LCD_W) + shiftX] = font[(*String) * fontW + i - asciiShift];

		String++;
		x++;

	}
}



//Print ICON(8*8) x=0-15 y=0-7
void GLCD_ICON_Print(uint8_t x,uint8_t y,const uint8_t * ICON)
{
	int i;
	for(i=0;i<8;i++)
		GLCD_Buf[i+(x*8)+(y*128)]=ICON[i];
}

/* Toggle rectangle
 * Start point (x,y)
 * w -> width
 * h -> height
 */
void ToggleRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	uint8_t i;

	/* Check input parameters */
	if (
		x >= numCols ||
		y >= numRows
	) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= numCols) {
		w = numCols - x;
	}
	if ((y + h) >= numRows) {
		h = numRows - y;
	}

	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		ToggleLine(x, y + i, x + w, y + i);
	}
}

//Debug
void GLCD_Test(void)
{

}
