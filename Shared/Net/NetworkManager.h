#ifndef zenic_net_NetworkManager_h
#define zenic_net_NetworkManager_h

#if defined(ZENIC_WIN32)
#include "Win32/NetworkManager.h"
#elif defined(ZENIC_PS2)
#include "Ps2/NetworkManager.h"
#else
#error "Unsupported platform"
#endif

#endif
