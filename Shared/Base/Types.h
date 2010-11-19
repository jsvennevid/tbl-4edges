#ifndef zenic_Types_h
#define zenic_Types_h

#if defined(ZENIC_WIN32)
	#include "Win32/Types.h"
#elif defined(ZENIC_PS2)
	#include "Ps2/Types.h"
#elif defined(ZENIC_PSP)
	#include "Psp/Types.h"
#else
	#error "Unsupported platform"
#endif

#ifndef ZENIC_RESTRICT
#define ZENIC_RESTRICT
#endif

#endif
