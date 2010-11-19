#ifndef zenic_Timer_h
#define zenic_Timer_h

#if defined(ZENIC_WIN32)
	#include "Win32/Timer.h"
#elif defined(ZENIC_PS2)
	#include "Ps2/Timer.h"
#else
	#error "Unsupported platform"
#endif

#endif
