#include <stdint.h>
#include <stdbool.h>
#include "mcp23s17.h"
#include "main.h"
#include "stm32f1xx_hal.h"

uint8_t expanderOk = 0;

void expanderSelect(void) {HAL_GPIO_WritePin(EXP_GPIO, EXP_PIN, 0);}
void expanderDeselect(void) {HAL_GPIO_WritePin(EXP_GPIO, EXP_PIN, 1);}

void expanderInit(void){
	expanderOk = 0;
	expanderDeselect();
	HAL_Delay(100);

	//expanderWriteReg(MCP23S17_IOCR, 0);

	expanderWriteReg(MCP23S17_DDR_A, 0);
	expanderWriteReg(MCP23S17_DDR_B, 0xFF);

	expanderWriteReg(MCP23S17_PUR_A, 0);
	expanderWriteReg(MCP23S17_PUR_B, 0xFF);

	expanderWriteReg(MCP23S17_POL_A, 0);
	expanderWriteReg(MCP23S17_POL_B, 0xFF);

	uint8_t testVal = 0xBE;
	setPolarity8(0, testVal);

	uint8_t readBack = getPolarity8(0);

	if (readBack == testVal){
		expanderOk = 1;
	}

	write8(0, expanderOk * 0xff);
	HAL_Delay(500);
	write8(0, 0);

}

uint8_t expanderReadReg(uint8_t addr) {
	  uint8_t txData[3];
	  txData[0] = READ_CMD | (EXP_ADDR << 1) | 0x40;
	  txData[1] = addr;
	  txData[2] = 0xFF; //dummy data for Tx

	  uint8_t rxData[3] = {0x55, 0x55, 0x55};

	  expanderSelect();
	  	  HAL_SPI_TransmitReceive(EXP_SPI_HANDLE, txData, rxData, 3, 1);
	  expanderDeselect();

	  return rxData[2];
}

void expanderWriteReg(uint8_t addr, uint8_t value) {
	  uint8_t txData[3];
	  txData[0] = WRITE_CMD | (EXP_ADDR << 1) | 0x40;
	  txData[1] = addr;
	  txData[2] = value;

	  expanderSelect();
	  	  HAL_SPI_Transmit(EXP_SPI_HANDLE, txData, 3, 1);
	  expanderDeselect();
}


void expanderPinMode(uint8_t pin, uint8_t mode) {
	uint8_t dataDirectionRegister = MCP23S17_DDR_A;
	if (pin > 7){
		dataDirectionRegister = MCP23S17_DDR_B;
		pin -= 8;
	}
	uint8_t val = expanderReadReg(dataDirectionRegister);

	uint8_t mask = 1 << pin;
	// only work with valid
	if ((mode == INPUT) || (mode == INPUT_PULLUP))
		val |= mask;

	else if (mode == OUTPUT)
		val &= ~mask;


	expanderWriteReg(dataDirectionRegister, val);
}


void pinMode8(uint8_t port, uint8_t value){
	if (!port) 	expanderWriteReg(MCP23S17_DDR_A, value);
	else 		expanderWriteReg(MCP23S17_DDR_B, value);
}

void write8(uint8_t port, uint8_t value) {
	if (!port) 	expanderWriteReg(MCP23S17_GPIO_A, value);
	else 		expanderWriteReg(MCP23S17_GPIO_B, value);
}

int read8(uint8_t port){
  if (!port) return expanderReadReg(MCP23S17_GPIO_A);
  return 			expanderReadReg(MCP23S17_GPIO_B);
}

void setPolarity8(uint8_t port,  uint8_t mask){
  if (!port) 	expanderWriteReg(MCP23S17_POL_A, mask);
  else 			expanderWriteReg(MCP23S17_POL_B, mask);
}

int getPolarity8(uint8_t port){
	if (!port)	 return expanderReadReg(MCP23S17_POL_A);
	return 				expanderReadReg(MCP23S17_POL_B);
}

void setPullup8(uint8_t port, uint8_t mask){
  if (!port) 	expanderWriteReg(MCP23S17_PUR_A, mask);
  else	 		expanderWriteReg(MCP23S17_PUR_B, mask);
}

void setInterrupt8(uint8_t port,  uint8_t mask){
  if (!port) 	expanderWriteReg(MCP23S17_POL_A, mask);
  else 			expanderWriteReg(MCP23S17_POL_B, mask);
}


