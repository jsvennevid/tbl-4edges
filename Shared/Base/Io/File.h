#ifndef zenic_File_h
#define zenic_File_h

#if defined(ZENIC_WIN32)
	#include "Win32/File.h"
#elif defined(ZENIC_PS2)
	#include "Ps2/File.h"
#elif defined(ZENIC_PSP)
	#include "Psp/File.h"
#else
	#error "Unsupported platform"
#endif

// TODO: rework File class to pure 64-bit I/O

#endif // zenic_File_h
