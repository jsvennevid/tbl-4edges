#ifndef zenic_net_SocketManager_h
#define zenic_net_SocketManager_h

#if defined(ZENIC_WIN32)
#include "Win32/SocketManager.h"
#elif defined(ZENIC_PS2)
#include "Ps2/SocketManager.h"
#else
#error "Unsupported platform"
#endif

#endif
