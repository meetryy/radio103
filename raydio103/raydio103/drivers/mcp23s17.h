#ifndef _MCP23S17_H
#define _MCP23S17_H

#include <stdint.h>
#include "stm32f1xx_hal.h"

#define WRITE_CMD 0
#define READ_CMD 1

#define	EXP_GPIO		(GPIOA)
#define	EXP_PIN			(GPIO_PIN_9)
#define EXP_ADDR		(0)
#define EXP_SPI_HANDLE		&hspi1 // extern-ed in main.h

enum types {OUTPUT, INPUT, INPUT_PULLUP};

//  Registers                         // description                 datasheet
#define MCP23S17_DDR_A        0x00    // Data Direction Register A   P18
#define MCP23S17_DDR_B        0x01    // Data Direction Register B   P18
#define MCP23S17_POL_A        0x02    // Input Polarity A            P18
#define MCP23S17_POL_B        0x03    // Input Polarity B            P18
#define MCP23S17_GPINTEN_A    0x04    // NOT USED interrupt enable   P19
#define MCP23S17_GPINTEN_B    0x05    // NOT USED
#define MCP23S17_DEFVAL_A     0x06    // NOT USED interrupt def      P19
#define MCP23S17_DEFVAL_B     0x07    // NOT USED
#define MCP23S17_INTCON_A     0x08    // NOT USED interrupt control  P20
#define MCP23S17_INTCON_B     0x09    // NOT USED
#define MCP23S17_IOCR         0x0A    // IO control register         P20
#define MCP23S17_IOCR2        0x0B    // NOT USED
#define MCP23S17_PUR_A        0x0C    // Pull Up Resistors A         P22
#define MCP23S17_PUR_B        0x0D    // Pull Up Resistors A         P22
#define MCP23S17_INTF_A       0x0E    // NOT USED interrupt flag     P22
#define MCP23S17_INTF_B       0x0F    // NOT USED
#define MCP23S17_INTCAP_A     0x10    // NOT USED interrupt capture  P23
#define MCP23S17_INTCAP_B     0x11    // NOT USED
#define MCP23S17_GPIO_A       0x12    // General Purpose IO A        P23
#define MCP23S17_GPIO_B       0x13    // General Purpose IO B        P23
#define MCP23S17_OLAT_A       0x14    // NOT USED output latch       P24
#define MCP23S17_OLAT_B       0x15    // NOT USED

//  IOCR bit masks (details datasheet P20)
#define MCP23S17_IOCR_BANK    0x80    // Controls how the registers are addressed.
#define MCP23S17_IOCR_MIRROR  0x40    // INT Pins Mirror bit.
#define MCP23S17_IOCR_SEQOP   0x20    // Sequential Operation mode bit.
#define MCP23S17_IOCR_DISSLW  0x10    // Slew Rate control bit for SDA output.
#define MCP23S17_IOCR_HAEN    0x08    // Hardware Address Enable bit (MCP23S17 only).
#define MCP23S17_IOCR_ODR     0x04    // Configures the INT pin as an open-drain output.
#define MCP23S17_IOCR_INTPOL  0x02    // This bit sets the polarity of the INT output pin.
#define MCP23S17_IOCR_NI      0x01    // Not implemented.


void expanderInit(void);
void expanderWriteReg(uint8_t addr, uint8_t value);
uint8_t expanderReadReg(uint8_t addr);


void expanderPinMode(uint8_t pin, uint8_t mode);
void pinMode8(uint8_t port, uint8_t value);
void write8(uint8_t port, uint8_t value);
int read8(uint8_t port);
void write8(uint8_t port, uint8_t value);
void setPolarity8(uint8_t port,  uint8_t mask);
int getPolarity8(uint8_t port);
void setPullup8(uint8_t port, uint8_t mask);
void setInterrupt8(uint8_t port,  uint8_t mask);


#endif
