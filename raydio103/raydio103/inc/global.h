#pragma once

#define USE_RAMFUNC

#ifdef USE_RAMFUNC
	#define _RAMFUNC		__attribute__ ((section(".RamFunc")))
#endif

