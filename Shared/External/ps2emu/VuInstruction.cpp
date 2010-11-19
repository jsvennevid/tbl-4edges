// (C) 2004 by Khaled Daham, <khaled@w-arts.com>

#include "VuInstruction.h"
#include <string.h>

namespace ps2emu
{

/////////////////////////////// PUBLIC ///////////////////////////////////////
void
VuInstruction::Reset() 
{
	memset(this, 0, sizeof(VuInstruction));

	addr        = 0;
    tics        = 0;
    breakpoint  = 0;
    SymbolIndex = -1;
}

}

/////////////////////////////// PRIVATE    ///////////////////////////////////
