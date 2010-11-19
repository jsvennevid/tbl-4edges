#ifndef zenic_Vector4_h

#if defined(ZENIC_WIN32)
	#include "Win32/Vector4.h"
#elif defined(ZENIC_PS2)
	#include "Ps2/Vector4.h"
#elif defined(ZENIC_PSP)
	#include "Psp/Vector4.h"
#else
	#error "Unsupported platform"
#endif

#endif // zenic_Vector4_h
