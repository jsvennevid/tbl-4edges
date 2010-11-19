// (C) 2004 by Khaled Daham, <khaled@w-arts.com>
#ifndef __MICROCODE__
#define __MICROCODE__
#include "VuInstructionDef.h"

namespace ps2emu
{

class MicroCode
{
public:
	VUInstructionDef Instr[200];
	int nInstructionDef;
	void DecThroughput();  
};

}

#endif
