#ifndef zenic_net_Socket_h
#define zenic_net_Socket_h

#if defined(ZENIC_WIN32)
#include "Win32/Socket.h"
#elif defined(ZENIC_PS2)
#include "Ps2/Socket.h"
#else
#error "Unsupported platform"
#endif

#endif
