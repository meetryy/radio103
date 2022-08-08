#include "stm32f1xx_hal.h"
#include "ssd1309.h"
#include "main.h"
#include <stdbool.h>

#define	DISP_CS_GPIO		(GPIOA)
#define	DISP_CS_PIN			(GPIO_PIN_12)
#define	DISP_DC_GPIO		(GPIOA)
#define	DISP_DC_PIN			(GPIO_PIN_15)

#define true	1
#define false	0

#define SSD1309_COL		128
#define SSD1309_ROW		64

#define DISP_SPI_HANDLE		&hspi1 // extern-ed in main.h

inline void displaySelect(void) 	{HAL_GPIO_WritePin(DISP_CS_GPIO, DISP_CS_PIN, 0);}
inline void displayDeselect(void) 	{HAL_GPIO_WritePin(DISP_CS_GPIO, DISP_CS_PIN, 1);}

inline void displaySetData(void) 	{HAL_GPIO_WritePin(DISP_DC_GPIO, DISP_DC_PIN, 1);}
inline void displaySetCmd(void) 	{HAL_GPIO_WritePin(DISP_DC_GPIO, DISP_DC_PIN, 0);}

/*
void spiTransmit(uint8_t data){
	displaySelect();
	SPI1->DR = data;
	while((SPI1->SR & SPI_SR_BSY))
	{};
	displayDeselect();
}
*/

void writeCmd(uint8_t Command2Write){
	displaySetCmd();
	displaySelect();
		HAL_SPI_Transmit(DISP_SPI_HANDLE, &Command2Write, 1, 1);
	displayDeselect();
}

void writeCmdDual(uint8_t cmd1, uint8_t cmd2){
	uint8_t cmd[2] = {cmd1, cmd2};
	displaySetCmd();
	displaySelect();
		HAL_SPI_Transmit(DISP_SPI_HANDLE, &cmd, 2, 1);
	displayDeselect();
}

void writeData(uint8_t Data2Write){
	displaySetData();
	displaySelect();
		HAL_SPI_Transmit(DISP_SPI_HANDLE, &Data2Write, 1, 1);
	displayDeselect();
}

void cmd_AddressingMode(uint8_t mode){
	writeCmd(0x20);			// Set Memory Addressing Mode
	writeCmd(mode);			//   Default => 2
}

void cmd_ColumnStartAddress(uint8_t address){
// Set Lower Column Start Address for Page Addressing Mode
// Set Higher Column Start Address for Page Addressing Mode
	writeCmdDual(0x00+address%16, 0x10+address/16);
}

void cmd_PageStartAddress(uint8_t page){
	writeCmd(0xB0|page);			// Set Page Start Address for Page Addressing Mode
}

void cmd_StartLine(uint8_t line){
	writeCmd(0x40|line);			// Set Display Start Line
	//   Default => 64 (0x00)
}


void cmd_ContrastControl(uint8_t contrast){
	writeCmd(0x81);			// Set Contrast Control for Bank 0
	writeCmd(contrast);		//
}


void cmd_SegmentRemap(bool remap){
	writeCmd(0xA1 + 0x01*remap);	// 0xA1 => Column Address 0 Mapped to SEG131									// 0xA0 => Column Address 0 Mapped to SEG0
}


void cmd_ComRemap(bool remap){
	writeCmd(0xC0+0x08*remap);	// 0xC0 (0x00) => Scan from COM0 to 63
}


void cmd_EntireDisplayON(bool on){
//     0xA5 => Entire Display On
	writeCmd(0xA4+0x01*on);	//     0xA4 => Normal Display
}


void cmd_InverseDisplay(bool inverse){
//     0xA7 => Inverse Display On
		writeCmd(0xA6+inverse);	//     0xA6 => Normal Display
}


void cmd_MultiplexRatio(uint8_t mux){
	writeCmd(0xA8);			// Set Multiplex Ratio
	writeCmd(mux-1);			//   Default => 64 (1/64 Duty)
}

void cmd_DisplayOn(bool on){
	if (on)
		writeCmd(0xAF);	//     0xAF => Display On
	else
		writeCmd(0xAE);	//     0xAE => Display Off
}


void cmd_DisplayOffset(uint8_t offset){
	writeCmd(0xD3);			// Set Display Offset
	writeCmd(offset);		//   Default => 0
}

void cmd_DisplayClock(uint8_t value){
	writeCmd(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	writeCmd(value);		// Default => 0x70
	//     D[3:0] => Display Clock Divider
	//     D[7:4] => Oscillator Frequency
}


void cmd_PrechargePeriod(uint8_t value){
	writeCmd(0xD9);			// Set Pre-Charge Period
	writeCmd(value);		//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
	//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
	//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


void cmd_ComPins(uint8_t config){
	writeCmd(0xDA);				// Set COM Pins Hardware Configuration
	writeCmd((config<<4)|0x02);	// Default => 0x12
}

void cmd_Vcomh(uint8_t value){
	writeCmd(0xDB);			// Set VCOMH deselect Level
	writeCmd(value);		// Default => 52 (0.78*VCC)
}

#include "gfx.h"

int currPage = 0;
bool lcdUpdateAllowed = 0;
bool lcdDmaDone = 1;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi1){
	displaySetCmd();
	displayDeselect();
	lcdDmaDone = 1;
}

/*
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi1);
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi1);
void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi1);
void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi1);
void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi1);
*/

void ssd1309PageUpdRoutine(void){
	/*
	//ssd1309StartDMAPage(currPage);
	if (lcdUpdateAllowed) {
		// don't update LCD this iteration, let gfx driver update framebuffer
		return;
	}

	cmd_PageStartAddress(currPage);
	cmd_ColumnStartAddress(0);

	if (lcdDmaDone){
		displaySetData();
		displaySelect();
		lcdDmaDone = 0;
		HAL_SPI_Transmit_DMA(DISP_SPI_HANDLE, &GLCD_Buf[128*currPage], SSD1309_COL);

		currPage++;
			if (currPage > 7){
				currPage = 0;
				lcdUpdateAllowed = 1;
			}

	}
*/

	HAL_SPI_Transmit_DMA(DISP_SPI_HANDLE, &GLCD_Buf[0], SSD1309_COL*8);

}

/*
void ssd1309Update(void){
	unsigned char i,j;
	cmd_PageStartAddress(currPage);
	cmd_ColumnStartAddress(0);


	//for(i=0;i<(SSD1309_ROW/8);i++)

	 displaySetData();
	 displaySelect();
	 	 // TODO: start DMA here
	 	 HAL_SPI_Transmit(DISP_SPI_HANDLE, &GLCD_Buf[128*currPage], SSD1309_COL, 10);
	 displaySetCmd();
	 displayDeselect();


}
*/

#define	LCD_ROTATE_180	(0)
#define	LCD_INVERT		(0)

void ssd1309Init(void){
	displayDeselect();
	HAL_Delay(100);

	cmd_DisplayOn(false);				// Display Off
	cmd_DisplayClock(0xF0);				// Set Clock as 80 Frames/Sec
	cmd_MultiplexRatio(SSD1309_ROW);	// 1/64 Duty (16-64)
	cmd_DisplayOffset(0);				// Shift Mapping RAM Counter (0~63)
	cmd_StartLine(0);					// Set Mapping RAM Display Start Line (0~63)
	cmd_AddressingMode(0);				// Set Page Addressing Mode
	cmd_SegmentRemap(LCD_ROTATE_180);			// standard segment mapping
	cmd_ComRemap(!LCD_ROTATE_180);				// standard col mapping
	cmd_ComPins(1);						// default com pins
	cmd_ContrastControl(10);			// Set SEG Output Current //Brightness
	cmd_PrechargePeriod(0xF1);	 		// Set Pre-Charge as 16 Clocks & Discharge as 1 Clocks
	cmd_Vcomh(55);						// Set VCOM Deselect Level //55
	cmd_EntireDisplayON(false);			// Disable Entire Display On
	cmd_InverseDisplay(LCD_INVERT);			// Disable Inverse Display
	//ssd1309Update();					// Clear Screen
	cmd_DisplayOn(true);				// Display On
}
