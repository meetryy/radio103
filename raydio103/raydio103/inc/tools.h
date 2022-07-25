#pragma once
#include <stdbool.h>
#include "global.h"

#define DBG_UART

extern long done;
extern bool uartTxDone;

enum {COLOR_RED, COLOR_GREEN, COLOR_BLUE};

//extern const UART_HandleTypeDef* dbgUartPtr;


void dbgTx(char* text, int len);

void debugPrintColor(int color, const char *fmt, ...);
void debugPrint(const char *fmt, ...);
void debugClearTerminal(void);
void debugPrintFast(const char *fmt, ...);

void testMethod(void);
float msPerOperation(int times);

void debugPinInit(void);
void debugPinSet(bool i);
void debugInit(void);


