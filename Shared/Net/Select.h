#ifndef zenic_net_Select_h
#define zenic_net_Select_h

#if defined(ZENIC_WIN32)
#include "Win32/Select.h"
#elif defined(ZENIC_PS2)
#include "Ps2/Select.h"
#else
#error "Unsupported platform"
#endif


#endif
