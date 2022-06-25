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

#define CS_PIN 	GPIO_PIN_6
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


uint8_t		buf[2048] = {0};
uint16_t	bufPtr = 0;


void ST7920_SendData2 (uint8_t data)
{

	//ST7920_SendCmd(0x03);
	//ST7920_SendCmd(0x40);



	for(int j=0;j<32;j++) {


		ST7920_SendCmd(0x80 | j); // 6-bit (0..63)
		ST7920_SendCmd(0x80 | 0); // 4-bit (0..15)

	    //SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3));

		int  b=0x55;
	    //SPI.transfer(0xFA); // data
	    buf[bufPtr] = 0xFA; bufPtr++;

	    for(int i=0; i<8;i++) {  // 16 bytes from line #0+
	      int b = 0x55;

	      buf[bufPtr] = (b & 0xF0); bufPtr++;
	      buf[bufPtr] = (b << 4); bufPtr++;

	    }

	      b=0xFF;

	    for(int i=0;i<16;i++) {  // 16 bytes from line #32+

	      buf[bufPtr] = (b & 0xF0); bufPtr++;
		  buf[bufPtr] = (b << 4); bufPtr++;
	    }


	    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);  // PUll the CS high
	     	HAL_SPI_Transmit(&hspi1, (uint8_t*)&buf, 32, 2);
	     HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);  // PUll the CS LOW
	     bufPtr = 0;


	    //
	  }





}


void ST7920_SendString(int row, int col, char* string)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0x90;
            break;
        case 2:
            col |= 0x88;
            break;
        case 3:
            col |= 0x98;
            break;
        default:
            col |= 0x80;
            break;
    }

    ST7920_SendCmd(col);

    while (*string)
    	{
    		ST7920_SendData(*string++);
    	}
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
	int Ystart = 0+half*64;
	int Yend = 64+half*64;



	for(y=Ystart;y<Yend;y++)
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

	ST7920_DrawBitmap(GLCD_Buf, lastHalf);
	//ST7920_Switch(lastHalf);
	//lastHalf = !lastHalf;

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
	//HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);  // RESET=0
	//HAL_Delay(10);   // wait for 10ms
	//HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);  // RESET=1

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
    HAL_Delay(1);  // 1ms delay


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

// Toggle Pixel
void TogglePixel(uint8_t x, uint8_t y)
{
  if (y < numRows && x < numCols)
  {
		if((GLCD_Buf[(x)+((y/8)*128)]>>y%8)&0x01)
			ClearPixel(x,y);
		else
			SetPixel(x,y);
  }
}
/* draw a line
 * start point (X0, Y0)
 * end point (X1, Y1)
 */
void DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  int dx = (x1 >= x0) ? x1 - x0 : x0 - x1;
  int dy = (y1 >= y0) ? y1 - y0 : y0 - y1;
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  for (;;)
  {
    SetPixel(x0, y0);
    if (x0 == x1 && y0 == y1) break;
    int e2 = err + err;
    if (e2 > -dy)
    {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}

/* Draw rectangle
 * start point (x,y)
 * w -> width
 * h -> height
 */
void DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
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

	/* Draw 4 lines */
	DrawLine(x, y, x + w, y);         /* Top line */
	DrawLine(x, y + h, x + w, y + h); /* Bottom line */
	DrawLine(x, y, x, y + h);         /* Left line */
	DrawLine(x + w, y, x + w, y + h); /* Right line */
}

/* Draw filled rectangle
 * Start point (x,y)
 * w -> width
 * h -> height
 */
void DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
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
		DrawLine(x, y + i, x + w, y + i);
	}
}

/* draw circle
 * centre (x0,y0)
 * radius = radius
 */
void DrawCircle(uint8_t x0, uint8_t y0, uint8_t radius)
{
  int f = 1 - (int)radius;
  int ddF_x = 1;

  int ddF_y = -2 * (int)radius;
  int x = 0;

  SetPixel(x0, y0 + radius);
  SetPixel(x0, y0 - radius);
  SetPixel(x0 + radius, y0);
  SetPixel(x0 - radius, y0);

  int y = radius;
  while(x < y)
  {
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    SetPixel(x0 + x, y0 + y);
    SetPixel(x0 - x, y0 + y);
    SetPixel(x0 + x, y0 - y);
    SetPixel(x0 - x, y0 - y);
    SetPixel(x0 + y, y0 + x);
    SetPixel(x0 - y, y0 + x);
    SetPixel(x0 + y, y0 - x);
    SetPixel(x0 - y, y0 - x);
  }
}

// Draw Filled Circle
void DrawFilledCircle(int16_t x0, int16_t y0, int16_t r)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    SetPixel(x0, y0 + r);
    SetPixel(x0, y0 - r);
    SetPixel(x0 + r, y0);
    SetPixel(x0 - r, y0);
    DrawLine(x0 - r, y0, x0 + r, y0);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        DrawLine(x0 - x, y0 + y, x0 + x, y0 + y);
        DrawLine(x0 + x, y0 - y, x0 - x, y0 - y);

        DrawLine(x0 + y, y0 + x, x0 - y, y0 + x);
        DrawLine(x0 + y, y0 - x, x0 - y, y0 - x);
    }
}

// Clear Filled Circle
void ClearFilledCircle(int16_t x0, int16_t y0, int16_t r)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    ClearPixel(x0, y0 + r);
    ClearPixel(x0, y0 - r);
    ClearPixel(x0 + r, y0);
    ClearPixel(x0 - r, y0);
    ClearLine(x0 - r, y0, x0 + r, y0);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ClearLine(x0 - x, y0 + y, x0 + x, y0 + y);
        ClearLine(x0 + x, y0 - y, x0 - x, y0 - y);

        ClearLine(x0 + y, y0 + x, x0 - y, y0 + x);
        ClearLine(x0 + y, y0 - x, x0 - y, y0 - x);
    }
}

// Draw Traingle with coordimates (x1, y1), (x2, y2), (x3, y3)
void DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
	/* Draw lines */
	DrawLine(x1, y1, x2, y2);
	DrawLine(x2, y2, x3, y3);
	DrawLine(x3, y3, x1, y1);
}

// Draw Filled Traingle with coordimates (x1, y1), (x2, y2), (x3, y3)
void DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
	curpixel = 0;

#define ABS(x)   ((x) > 0 ? (x) : -(x))

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay){
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
		DrawLine(x, y, x3, y3);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

//Clear Line
void ClearLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  int dx = (x1 >= x0) ? x1 - x0 : x0 - x1;
  int dy = (y1 >= y0) ? y1 - y0 : y0 - y1;
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  for (;;)
  {
    ClearPixel(x0, y0);
    if (x0 == x1 && y0 == y1) break;
    int e2 = err + err;
    if (e2 > -dy)
    {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}

//Toggle Line
void ToggleLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  int dx = (x1 >= x0) ? x1 - x0 : x0 - x1;
  int dy = (y1 >= y0) ? y1 - y0 : y0 - y1;
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  for (;;)
  {
    TogglePixel(x0, y0);
    if (x0 == x1 && y0 == y1) break;
    int e2 = err + err;
    if (e2 > -dy)
    {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}


uint16_t character[] = {
		//0xFFFF, 0x5555, 0xAAAA, 0xFFFF, 0x5555, 0xAAAA,0xFFFF, 0x5555, 0xAAAA, 0xFFFF, 0x5555, 0xAAAA,0xFFFF, 0x5555, 0xAAAA, 0xFFFF, 0x5555, 0xAAAA
		0x00, 0x00, 0x00, 0xF8, 0x08, 0x08, 0x08, 0x08, 0x08, 0xF0, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x3F, 0x01, 0x01, 0x03, 0x07, 0x19, 0x30, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,


};

int fontW = 14;
int fontH = 14;

void GLCD_Font_New(uint8_t x, uint8_t y, char * String)
{
	/*
	for (int i=0; i<8; i++){
		//GLCD_Buf[128*i+1] = 0x80;
		for (int j=1; j<5; j++){
			int o = 0x00;
			if (i%2)
				o = 0xff;
			GLCD_Buf[128*i+j] = o;
		}
	}

*/

	int tileY = y/8;
	int Yshift = (y % 8);

	for (int i=0; i < fontW; i++){
		//uint32_t fullCol = (character[i]) | character[i+fontH]  << 8 | character[i+fontH*2]  << 16;

		uint32_t fullCol = (character[i]) | character[i+fontH]  << 8 | character[i+fontH*2]  << 16;

		int pixLeft = fontH;
		int tileCount = 0;
		while (pixLeft > 0){

			//int pixInThisTile = 8;
			//if (fontH < 8) pixInThisTile = fontH;
			//if (tileCount == 0) pixInThisTile = 8 - Yshift;
			//if (pixLeft < 8) pixInThisTile = pixLeft;

			uint32_t a = (fullCol << (Yshift)) >> (8 * tileCount);

			GLCD_Buf[x + i + 128 * tileCount +  128 * tileY ] = a;
			pixLeft -= 8 - Yshift*(tileCount == 0);
			tileCount++;
		}



		//int a = (fullCol << Yshift);

		//for (int tile=0; tile<8; tile++){
			//uint32_t b = a >> (8 * tile) & 0xFF;

			//uint32_t a = (fullCol << (Yshift)) >> (8 * tileCount);
			//GLCD_Buf[x + i + 128*tileCount +  1280*tileY] = a;
			//GLCD_Buf[10 + i + 128 * tileY + 128*2] = b;
		//}


	}
/*
	GLCD_Buf[1] = 0x0f;
	GLCD_Buf[2] = 0x0f;
	GLCD_Buf[3] = 0xf0;
	GLCD_Buf[4] = 0xf0;

	int X = x;
	int Y = y;
	int YshiftInBox = (y % 8);
	int boxY = y / 8;
	int boxX = x;

	for (int charX = 0; charX <fontW; charX++){
		int YpixLeft = fontH;
		//int YposInBox = YshiftInBox;
		//int boxN = 0;
		int YtoFiliInBox = 0;
		int charYshift = YshiftInBox;

		while (YpixLeft > 0){

			if (YpixLeft > 8) YtoFiliInBox = 8 - YshiftInBox;
			else YtoFiliInBox = YpixLeft;

			//YtoFiliInBox = 8 - YshiftInBox;
			GLCD_Buf[(boxX)+(boxY*128)] |= (character[charX] << (charYshift)) & 0xFF;
			YpixLeft -= YtoFiliInBox;
			if (YpixLeft > 0) {
				//boxN++;
				boxY++;
				charYshift += YtoFiliInBox;
				YshiftInBox = 0;
			}
		}

		boxX++;
		boxY = y / 8;
		YshiftInBox = (y % 8);
	}
*/
}


//Print Fonted String x=0-15 y=0-7
void GLCD_Font_Print(uint8_t x,uint8_t y,char * String)
{
	int shiftX = 0;
	int shiftY = 1;

	int i;
	while(*String)
	{
		for(i=0;i<8;i++)
			GLCD_Buf[i+(x*8)+(y*128)+shiftX/*+shiftY*32*/]=Font[(*String)*8+i-33*8];
		String++;
		x++;
		/*
		if(x>15)
		{
			x=0;
			y++;
		}
		*/
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
