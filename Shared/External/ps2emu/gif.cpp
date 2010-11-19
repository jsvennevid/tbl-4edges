#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>

#include "gif.h"

namespace ps2emu
{

static const int PRIM      = 0x0;
static const int RGBAQ     = 0x1;
static const int ST        = 0x2;
static const int UV        = 0x3;
static const int XYZF2     = 0x4;
static const int XYZ2      = 0x5;
static const int TEX0_1    = 0x6;
static const int TEX0_2    = 0x7;
static const int CLAMP_1   = 0x8;
static const int CLAMP_2   = 0x9;
static const int FOG       = 0xA;
static const int XYZF3     = 0xC;
static const int XYZ3      = 0xD;
static const int AD        = 0xE;
static const int NOP       = 0xF;

static const std::string tRegisters[16] = {
    "PRIM", "RGBAQ", "ST", "UV", "XYZF2", "XYZ2", 
    "TEX0_1", "TEX0_2", "CLAMP_1", "CLAMP_2", "FOG",
    "RSRVD", "XYZF3", "XYZ3", "AD", "NOP"
};

static const std::string tGIFTag[7] = {
    "REGS", "NREG", "FLG", "PRIM", "PRE", "EOP", "NLOOP"
};

static const std::string tPRIM[7] = {
    "Point", "Line", "Line strip", "Triangle",
    "Triangle strip", "Triangle fan", "Sprite"
};

static const std::string tIIP[2] = {
    "Flat Shading", "Gouraud Shading"
};

static const std::string tTME[2] = {
    "Texture mapping OFF", "Texture mapping ON"
};

static const std::string tFGE[2] = {
    "Fogging OFF", "Fogging ON"
};

static const std::string tABE[2] = {
    "Alpha Blending OFF", "Alpha Blending ON"
};

static const std::string tAA1[2] = {
    "Antialiasing OFF", "Antialiasing ON"
};

static const std::string tFST[2] = {
    "STQ", "UV"
};

static const std::string tCTXT[2] = {
    "Context 1", "Context 2"
};

static const std::string tFIX[2] = {
    "Unfixed", "Fixed"
};

// GIF_STAT
static const std::string tM3R[2] = {
    "Enable ( Initial value )",
    "Disable ( Masked by MR3 flag of GIF_MODE register.)"
};
static const std::string tM3P[2] = {
    "Enable ( Initial value )",
    "Disable ( Masked by MASKP3 of VIF.)"
};
static const std::string tIMT[2] = {
    "Continuous transfer mode ( Initial value )",
    "Intermittent transfer mode"
};
static const std::string tPSE[2] = {
    "Normal ( Initial value )",
    "Temporary stop state by PSE flag of GIF_CTRL register"
};
static const std::string tIP3[2] = {
    "No interrupted transer via PATH3 (Initial value)",
    "Interrupted transfer via PATH3"
};
static const std::string tP3Q[2] = {
    "No request to wait for processing in PATH3 (Initial value)",
    "Request to wait for processing in PATH3"
};
static const std::string tP2Q[2] = {
    "No request to wait for processing in PATH2 (Initial value)",
    "Request to wait for processing in PATH2"
};
static const std::string tP1Q[2] = {
    "No request to wait for processing in PATH1 (Initial value)",
    "Request to wait for processing in PATH1"
};
static const std::string tOPH[2] = {
    "Idle state (Initial value)",
    "Outputting data"
};
static const std::string tAPATH[4] = {
    "Idle state (Initial value)",
    "Transferring data via PATH1",
    "Transferring data via PATH2",
    "Transferring data via PATH3"
};
static const std::string tDIR[2] = {
    "EE to GS (Initial value)",
    "GS to EE"
};

/////////////////////////////// PUBLIC ///////////////////////////////////////

Gif::Gif() : SubSystem(GIFnREGISTERS) {
}

Gif::Gif(uint32 *data, uint32 size) : SubSystem(GIFnREGISTERS) {
    uint32 i;
    for(i = 0; i < size/16; i++) {
        gifData[i].x = data[i*4+0];
        gifData[i].y = data[i*4+1];
        gifData[i].z = data[i*4+2];
        gifData[i].w = data[i*4+3];
    }
    nloop   = 0;
    eop     = 0;
    nreg    = 0;
    counter = 0;
    curNreg = 0;
    curNreg = 0;
    REGISTERS = new uint32[GIFnREGISTERS];
}

void
Gif::SetRegisters(uint32 *data, uint32 size) {
    uint32 i;
    for(i = 0; i < size/8; i++) {
        REGISTERS[i] = *(data++);
    }
}

bool
Gif::Unpack() {
    UnpackNloop();
    UnpackNreg();
    UnpackFlag();
    UnpackPre();
    UnpackPrim();
    UnpackRegisters();
    counter = counter + 1;
    return Validate();
}

void
Gif::UnpackRegisters(void) {
	uint32 i;
    u64 regs = ((u64)gifData[counter].w<<32)+(u64)gifData[counter].z;
    for(i = 0; i < 16; i++) {
        registers[i] = 0xf;
    }
    for(i = 0; i < nreg; i++) {
        registers[i] = u8(regs & 0xf);
        regs = regs >> 4;
    } 
}

bool
Gif::Validate(void) {
    uint32 i;
    if (nloop > 1024) {
        return false;
    }
    for(i = 0; i < 16; i++) {
        if(registers[i] == 0xf) {
            if ( i == nreg ) {
                return true;
            } else {
                return false;
            }
        } 
    }
    return false;
}

std::vector<std::string>
Gif::NloopData() {
    std::string tag;
    std::vector<std::string> v;
    uint32 i;
    char x[32], y[32], z[32], w[32];
    if ( nloop == 0 ) {
        return v;
    }

    for(i = 0; i < nreg; i++) {
        v.push_back((std::string)tRegisters[registers[i]]);
        switch (registers[i]) {
            case PRIM:
                sprintf(x, "0x%x", gifData[counter].x);
                v.push_back((std::string)x);
                break;
            case RGBAQ:
                sprintf(x, "0x%x, ", gifData[counter].x);
                sprintf(y, "0x%x, ", gifData[counter].y);
                sprintf(z, "0x%x, ", gifData[counter].z);
                sprintf(w, "0x%x", gifData[counter].w);
                v.push_back("R: " + (std::string)x +
                       "G: " + (std::string)y +
                       "B: " + (std::string)x +
                       "A: " + (std::string)w);
                break;
            case ST:
                sprintf(x, "%ff, ", *((float *)&gifData[counter].x));
                sprintf(y, "%ff, ", *((float *)&gifData[counter].y));
                sprintf(z, "%ff\n", *((float *)&gifData[counter].z));
                v.push_back((std::string)x +
                        (std::string)y +
                        (std::string)z);
                break;
            case UV:
                sprintf(x, "%u, ", (gifData[counter].x>>4));
                sprintf(y, "%u\n", (gifData[counter].y>>4));
                v.push_back("U: " + (std::string)x +
                       "V: " + (std::string)y);
                break;
            case XYZF2:
                sprintf(x, "%u, ", (gifData[counter].x>>4));
                sprintf(y, "%u, ", (gifData[counter].y>>4));
                sprintf(z, "%u, ", (gifData[counter].z>>4));
                sprintf(w, "%u", ((gifData[counter].w>>4)&0xff));
                v.push_back("X: " + (std::string)x +
                       "Y: " + (std::string)y +
                       "Z: " + (std::string)z +
                       "F: " + (std::string)w);
                break;
            case XYZ2:
                sprintf(x, "%u, ", ((gifData[counter].x)>>4)-(uint32)xoffset);
                sprintf(y, "%u, ", ((gifData[counter].y)>>4)-(uint32)yoffset);
                sprintf(z, "%u", gifData[counter].z);
                v.push_back("X: " + (std::string)x +
                       "Y: " + (std::string)y +
                       "Z: " + (std::string)z);
                break;
            case TEX0_1:
                v.push_back("");
                break;
            case TEX0_2:
                v.push_back("");
                break;
            case CLAMP_1:
                v.push_back("");
                break;
            case CLAMP_2:
                v.push_back("");
                break;
            case FOG:
                sprintf(w, "%u, ", (gifData[counter].w&0xFF));
                v.push_back((std::string)w);
                break;
            case XYZF3:
                sprintf(x, "%d, ", (gifData[counter].x>>4));
                sprintf(y, "%d, ", (gifData[counter].y>>4));
                sprintf(z, "%u, ", (gifData[counter].z>>4));
                sprintf(w, "%u", ((gifData[counter].w>>4)&0xff));
                v.push_back((std::string)x + " " +
                        (std::string)y + " " +
                        (std::string)z + " " +
                        (std::string)w);
                break;
            case XYZ3:
                sprintf(x, "%d, ", (gifData[counter].x>>4));
                sprintf(y, "%d, ", (gifData[counter].y>>4));
                sprintf(z, "%u", gifData[counter].z);
                v.push_back((std::string)x + " " +
                        (std::string)y + " " +
                        (std::string)z);
                break;
            case AD:
                v.push_back("");
                break;
            case NOP:
                v.push_back("");
                break;
            default:
                break;
        }
        counter = counter+1;
    }
    curNloop = curNloop - 1;
    return v;
}

std::string
Gif::GetReadableTag() {
    std::string tag;
    uint32 i;
    char x[32], y[32], z[32], w[32];
    if ( nloop == 0 ) {
        return NULL;
    }

    for(i = 0; i < nreg; i++) {
        tag += (std::string)tRegisters[registers[i]] + "\t";
        sprintf(x, "%f, ", *((float *)&gifData[counter].x));
        sprintf(y, "%f, ", *((float *)&gifData[counter].y));
        sprintf(z, "%f, ", *((float *)&gifData[counter].z));
        sprintf(w, "%f", *((float *)&gifData[counter].w));
        tag += (std::string)x + (std::string)y + (std::string)z + (std::string)w + "\n";
        counter = counter+1;
    }
    curNloop = curNloop - 1;
    return tag;
}

std::string
Gif::RegsAsString(void) {
    std::string regs;
    uint32 i;
    for(i = 0; i < nreg; i++) {
        regs += (std::string)tRegisters[registers[i]];
    }
	return regs;
}

uint32
Gif::GetNreg(void) {
    return nreg;
}

uint32
Gif::GetNloop(void) {
    return nloop;
}

/////////////////////////////// PRIVATE    ///////////////////////////////////
// void
// Gif::Init(void) {
// }

void
Gif::UnpackFlag(void) {
    flag = ((gifData[counter].y>>26)&0x3);
}

void
Gif::UnpackPrim(void) {
    prim = ((gifData[counter].y>>15)&0x3ff);
}

void
Gif::UnpackPre(void) {
    pre = ((gifData[counter].y>>14)&0x1);
}

void
Gif::UnpackEop(void) {
    eop = ((gifData[counter].x>>15)&0x1);
}

void
Gif::UnpackNloop(void) {
   nloop = (gifData[counter].x&0x7FFF);
   curNloop = nloop;
}

void
Gif::UnpackNreg(void) {
    nreg = ((gifData[counter].y>>28));
    curNreg = nreg;
}

std::vector<std::string>
Gif::TagAsString(void) {
    std::string reg;
    std::vector<std::string> v;
    char val[100];
    v.push_back(tGIFTag[0]);
	uint32 i;
    // wanker code
    for (i = 15; i > 8; i--) {
        reg += (std::string)tRegisters[registers[i]] + ", ";
    }
    reg += (std::string)tRegisters[registers[8]];
    v.push_back(reg);
    reg = "";
    v.push_back(reg);

    for(i = 7; i > 0; i--) {
        reg += (std::string)tRegisters[registers[i]] + ", ";
    }
    reg += (std::string)tRegisters[registers[0]];
    v.push_back(reg);

    v.push_back((std::string)tGIFTag[1]);
    sprintf(val, "%d", nreg);
    v.push_back((std::string)val);

    v.push_back((std::string)tGIFTag[2]);
    sprintf(val, "0x%x", flag);
    v.push_back((std::string)val);

    // prim
    v.push_back((std::string)tGIFTag[3]);
    // v.push_back("");
    ParsePrim(v);

    v.push_back((std::string)tGIFTag[4]);
    sprintf(val, "%d", pre);
    v.push_back((std::string)val);

    v.push_back((std::string)tGIFTag[5]);
    sprintf(val, "%d", eop);
    v.push_back((std::string)val);

    v.push_back((std::string)tGIFTag[6]);
    sprintf(val, "%d", nloop);
    v.push_back((std::string)val);
    return v;
}

void
Gif::ParsePrim(std::vector<std::string> &v) {
    v.push_back((std::string)tPRIM[prim&0x7]);
    v.push_back("");
    v.push_back(tIIP[(prim&0x8)>>3]);
    v.push_back("");
    v.push_back(tTME[(prim&0x10)>>4]);
    v.push_back("");
    v.push_back((std::string)tFGE[(prim&0x20)>>5]);
    v.push_back("");
    v.push_back((std::string)tABE[(prim&0x40)>>6]);
    v.push_back("");
    v.push_back((std::string)tAA1[(prim&0x80)>>7]);
    v.push_back("");
    v.push_back((std::string)tFST[(prim&0x100)>>8]);
    v.push_back("");
    v.push_back((std::string)tCTXT[(prim&0x200)>>9]);
    v.push_back("");
    v.push_back((std::string)tFIX[(prim&0x400)>>10]);
}

const std::vector<std::string>
Gif::GetRegisterText(const int reg) {
    switch(reg) {
        case GIF_STAT:
            return UnpackStat(reg);
            break;
        case GIF_TAG0:
            return UnpackTag0(reg);
            break;
        case GIF_TAG1:
            return UnpackTag1(reg);
            break;
        case GIF_TAG2:
            return UnpackTag2(reg);
            break;
        case GIF_TAG3:
            return UnpackTag3(reg);
            break;
        case GIF_CNT:
            return UnpackCnt(reg);
            break;
        case GIF_P3CNT:
            return UnpackP3cnt(reg);
            break;
        case GIF_P3TAG:
            return UnpackP3tag(reg);
            break;
        default:
            std::vector<std::string> v;
            return v;
            break;
    }
}

std::vector<std::string>
Gif::UnpackStat(const int reg) {
    std::vector<std::string> v;
    v.push_back("M3R");
    v.push_back(tM3R[(REGISTERS[reg]&0x1)]);
    v.push_back("M3P");
    v.push_back(tM3P[(REGISTERS[reg]&0x2)]);
    v.push_back("IMT");
    v.push_back(tIMT[(REGISTERS[reg]&0x4)]);
    v.push_back("PSE");
    v.push_back(tPSE[(REGISTERS[reg]&0x80)]);
    v.push_back("IP3");
    v.push_back(tIP3[(REGISTERS[reg]&0x20)]);
    v.push_back("P3Q");
    v.push_back(tP3Q[(REGISTERS[reg]&0x40)]);
    v.push_back("P2Q");
    v.push_back(tP2Q[(REGISTERS[reg]&0x80)]);
    v.push_back("P1Q");
    v.push_back(tP1Q[(REGISTERS[reg]&0x100)]);
    v.push_back("OPH");
    v.push_back(tOPH[(REGISTERS[reg]&0x200)]);
    v.push_back("APATH");
    v.push_back(tAPATH[(REGISTERS[reg]&0xC00)]);
    v.push_back("DIR");
    v.push_back(tDIR[(REGISTERS[reg]&0x1000)]);
    char val[100];
    sprintf(val, "%d", (REGISTERS[reg]&0x1F000000)>>24);
    v.push_back("FQC");
    v.push_back(val);
    return v;
}
std::vector<std::string>
Gif::UnpackTag0(const int reg) {
    std::vector<std::string> v;
    char val[100];
    sprintf(val, "%d", REGISTERS[reg]&0x00007FFF);
    v.push_back("NLOOP");
    v.push_back(val);
    sprintf(val, "%d", (REGISTERS[reg]&0x00008000)>>15);
    v.push_back("EOP");
    v.push_back(val);
    sprintf(val, "0x%x", (REGISTERS[reg]&0xFFFF0000)>>16);
    v.push_back("tag");
    v.push_back(val);
    return v;
}
std::vector<std::string>
Gif::UnpackTag1(const int reg) {
    std::vector<std::string> v;
    char val[100];

    v.push_back("tag");
    sprintf(val, "%d", REGISTERS[reg]&0x00003FFF);
    v.push_back(val);
    v.push_back("PRE");
    sprintf(val, "%d", (REGISTERS[reg]&0x00004000)>>14);
    v.push_back(val);
    v.push_back("PRIM");
    sprintf(val, "0x%x", (REGISTERS[reg]&0x03FF8000)>>15);
    v.push_back(val);
    v.push_back("FLG");
    sprintf(val, "%d", (REGISTERS[reg]&0x0C000000)>>26);
    v.push_back(val);
    v.push_back("NREG");
    sprintf(val, "%d", (REGISTERS[reg]&0xF0000000)>>28);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Gif::UnpackTag2(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("REGS");
    sprintf(val, "0x%x", REGISTERS[reg]);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Gif::UnpackTag3(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("REGS");
    sprintf(val, "0x%x", REGISTERS[reg]);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Gif::UnpackCnt(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("LOOPCNT");
    sprintf(val, "%d", REGISTERS[reg]&0x00007FFF);
    v.push_back(val);
    v.push_back("REGCNT");
    sprintf(val, "%d", (REGISTERS[reg]&0x000F0000)>>16);
    v.push_back(val);
    v.push_back("VUADDR");
    sprintf(val, "%d", (REGISTERS[reg]&0x3FF00000)>>20);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Gif::UnpackP3cnt(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("P3CNT");
    sprintf(val, "%d", REGISTERS[reg]&0x00007FFF);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Gif::UnpackP3tag(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("LOOPCNT");
    sprintf(val, "%d", REGISTERS[reg]&0x00007FFF);
    v.push_back(val);
    v.push_back("EOP");
    sprintf(val, "%d", (REGISTERS[reg]&0x00008000)>>15);
    v.push_back(val);
    return v;
}

}
