#pragma once
#include <stdbool.h>
extern bool lcdUpdateAllowed;

void ssd1309Init(void);
void ssd1309Update(void);
void ssd1309StartDMAPage(int page);
void ssd1309PageUpdRoutine(void);
