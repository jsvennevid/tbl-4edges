// (C) 2004 by Khaled Daham, <khaled@w-arts.com>

#include "VuIntReg.h"

namespace ps2emu
{

int16 VuIntReg::value(){
    return data.vi;
}

void VuIntReg::value(int16 v){
    data.vi = v;
    return;
}

}
