#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//#include "wx/std::string.h"
#include "vif.h"
#include "parser.h"
#include "vu.h"

namespace ps2emu
{

const int32 VIF_STATE_STOPPED   = 0;
const int32 VIF_STATE_READY     = 1;
const int32 VIF_STATE_RUN       = 2;

static const std::string tMOD[4] = {
    "No addition processing",
    "Offset mode (Row + dV -> VU Mem)",
    "Difference mode (Row + dV -> Row -> VU Mem)",
    "Undefined"
};
static const std::string tMII[2] = {
    "Unmask (i bit interrupt enable)",
    "Mask (i bit interrupt disable",
};
static const std::string tME0[2] = {
    "Unmask (stalls when error occurs.)",
    "Mask (ignores DMAtag Mismatch error.)"
};
static const std::string tME1[2] = {
    "Unmask (stalls when an error occurs.)",
    "Mask (considered as VIFcode NOP.)"
};

static std::string tUNPACK_MODES[16] = {
    "S32\n",
    "S16\n",
    "S8\n",
    "",
    "V2_32\n",
    "V2_16\n",
    "V2_8\n",
    "",
    "V3_32\n",
    "V3_16\n",
    "V3_8\n",
    "",
    "V4_32\n",
    "V4_16\n",
    "V4_8\n",
    "V4_5\n"
};

/////////////////////////////// PUBLIC ///////////////////////////////////////
int limit(int a, int max) { return(a > max ? max: a);}

Vif::Vif() : SubSystem(0) {
    Init();
}

Vif::Vif(int num) : SubSystem(num) {
    nREGISTERS = num;
    Init();
}

Vif::Vif(const char *filename, Parser* pParser, Vu* pVu) : SubSystem(0) {
    m_pParser = pParser;
    m_pVu = pVu;
    Init();
}

Vif::Vif(const char *filename, int num) : SubSystem(num) {
    nREGISTERS = num;
    Init();
}

// de struct or
Vif::~Vif() {
}

const int32
Vif::Open(const char* filename) {
    return E_OK;
}

const int32
Vif::Close() {
    return E_OK;
}

const int32
Vif::GetState() {
    return m_state;
}

const bool
Vif::IsStopped() {
    return true;
}

const bool
Vif::IsRunning() {
    return true;
}

/////////////////////////////// PRIVATE    ///////////////////////////////////
void
Vif::Init(void) {
	m_interrupt = false;
	vpu = 0;
	rVIF1_R0 = 0;
	rVIF1_R1 = 0;
	rVIF1_R2 = 0;
	rVIF1_R3 = 0;
	rVIF1_C0 = 0;
	rVIF1_C1 = 0;
	rVIF1_C2 = 0;
	rVIF1_C3 = 0;
	rVIF1_CYCLE = 0;
	rVIF1_MASK = 0;
	rVIF1_MODE = 0;
	rVIF1_ITOP = 0;
	rVIF1_ITOPS = 0;
	rVIF1_BASE = 0;
	rVIF1_OFST = 0;
	rVIF1_TOP = 0;
	rVIF1_TOPS = 0;
	rVIF1_MARK = 0;
	rVIF1_NUM = 0;
	rVIF1_CODE = 0;
	rVIF1_STAT = 0;
	rVIF1_FBRST = 0;
	rVIF1_ERR = 0;

	rVIF0_R0 = 0;
	rVIF0_R1 = 0;
	rVIF0_R2 = 0;
	rVIF0_R3 = 0;
	rVIF0_C0 = 0;
	rVIF0_C1 = 0;
	rVIF0_C2 = 0;
	rVIF0_C3 = 0;
	rVIF0_CYCLE = 0;
	rVIF0_MASK = 0;
	rVIF0_MODE = 0;
	rVIF0_ITOP = 0;
	rVIF0_ITOPS = 0;
	rVIF0_MARK = 0;
	rVIF0_NUM = 0;
	rVIF0_CODE = 0;
    m_vifCmdNum = 0;
    m_vifCmd = NULL;
    _WL = 0;
    _CL = 0;
    m_memIndex = 0;
    m_codeIndex = 0;
	m_trace = true;
}

// ------------------------------------------------------------------
// VIF commands
const int32
Vif::CmdNop(void) {
    m_vifCmd = VIF_NOP;
    if ( m_trace ) {
        //m_pLog->Trace(1, "NOP\n");
    }
    return E_OK;
}

const int32
Vif::CmdStcycl(const int32& data) {
    m_vifCmd = VIF_STCYCL;
    _WL = (data>>8)&0xff;
    _CL = data&0xff;
    if ( m_trace ) {
        m_pLog->Trace(1, "STCYCL\n");
        m_pLog->Trace(2, Log::Format("WL: %d, CL: %d\n", _WL, _CL));
    }
    return E_OK;
}

const int32
Vif::CmdItop(const int32& data) {
    if ( m_trace ) {
        //m_pLog->Trace(1, "ITOP\n");
    }
    m_vifCmd = VIF_ITOP;
    m_vifCmdNum = 0;
    rVIF0_ITOPS = data&0x3ff;
    rVIF1_ITOPS = data&0x3ff;
    return E_OK;
}

const int32
Vif::CmdStmod(const int32& data) {
    if ( m_trace ) {
        //m_pLog->Trace(1, "STMOD\n");
    }
    m_vifCmd = VIF_STMOD;
    rVIF0_MODE = data&0x3;
    rVIF1_MODE = data&0x3;
    return E_OK;
}

const int32
Vif::CmdMark(const int32& data) {
    if ( m_trace ) {
        //m_pLog->Trace(1, "MARK\n");
    }
    m_vifCmd = VIF_MARK;
    rVIF0_MARK = data&0xFFFF;
    rVIF1_MARK = data&0xFFFF;
    return E_OK;
}

const int32
Vif::CmdFlushE(void) {
    // TODO
    m_vifCmd = VIF_FLUSHE;
    if ( m_trace ) {
        m_pLog->Trace(1, "FLUSHE\n");
    }
    return E_OK;
}

const int32
Vif::CmdMpg(const int32& data) {
    //unsigned char  raw[4];
    uint32  lower;
    uint32  upper;
    uint32  index;
    char    uppline[128];
    char    lowline[128];
    char    uparam[128];
    char    lparam[128];

	memset(uppline, 0, 128);
	memset(lowline, 0, 128);
	memset(uparam, 0, 128);
	memset(lparam, 0, 128);

    m_vifCmd = VIF_MPG;
    m_vifCmdNum = (data>>16)&0xFF;
    _addr = data&0xffff;
    index = _addr;
    if ( m_vifCmdNum == 0 ) {
        m_vifCmdNum = 256;
    }
    if ( m_trace ) {
        m_pLog->Trace(1, Log::Format("MPG, Num = %d, Load addr = %d\n",
                m_vifCmdNum, _addr));
    }

	int i = 0;

    while ( m_vifCmdNum > 0 ) {
		
		
		//m_fin->read((char*)&raw, 4);
        //lower = ((uint32)raw[3]<<24)+ ((uint32)raw[2]<<16)+ ((uint32)raw[1]<<8)+ (uint32)raw[0];
		lower = m_stream.read<uint32>();
		//m_pLog->Trace(1, Log::Format("0x%x\n", lower); 
        //m_fin->read((char*)&raw, 4);
        //upper = ((uint32)raw[3]<<24) + ((uint32)raw[2]<<16)+ (((uint32)raw[1])<<8)+ ((uint32)raw[0]);
		upper = m_stream.read<uint32>();
		//m_pLog->Trace(1, Log::Format("0x%x\n", ((uint32)raw[0]<<24)+ ((uint32)raw[1]<<16)+ ((uint32)raw[2]<<8)+ (uint32)raw[3])); 
        m_pParser->dlower(&lower, lowline, lparam, index);
        m_pParser->dupper(&upper, uppline, uparam, index);
        m_pParser->insert(uppline, lowline, uparam, lparam, index, lower, upper);
		m_pLog->Trace(1, Log::Format("%d | %s %s %s %s\n", i++, uppline, uparam, lowline, lparam)); 
		index++;
        m_vifCmdNum--;
    }

    if ( m_vifCmdNum == 0 ) {
        m_vifCmd = NULL;
    }
    return E_OK;
}

// --------------------------------------------------------------------------
// function to extract VIF UNPACK data
const int32
Vif::CmdUnpack(void) {
    //char    rword[4], rhword[2], rbyte[1];
    uint32  word_x = 0, word_y = 0, word_z = 0, word_w = 0;
    for (;m_vifCmdNum > 0; m_vifCmdNum--) {
		if (m_stream.isFinished()) {
            m_pLog->Warning("Premature buffer\n");
            return E_FILE_EOF;
        }
        uint32 cycle = 0;
        uint32 write = 0;
        if( _CL > _WL ) {
            if ( write >= _WL ) {
                m_memIndex += _CL-_WL;
                cycle = 0;
                write = 0;
            }
        } else if ( _CL > _WL ) {
            if ( write >= _CL ) {
                for(uint32 cnt = 0; cnt < (_CL-_WL); cnt++) {
                    m_pVu->WriteMemX(m_memIndex, rVIF1_C0);
                    m_pVu->WriteMemY(m_memIndex, rVIF1_C1);
                    m_pVu->WriteMemZ(m_memIndex, rVIF1_C2);
                    m_pVu->WriteMemW(m_memIndex, rVIF1_C3);
                    m_memIndex++;
                }
                cycle = 0;
                write = 0;
            }
        }
        if ( m_trace ) {
            m_pLog->Trace(2, tUNPACK_MODES[m_unpack]);
        }
        switch(m_unpack) {
            case UNPACK_S32:
				word_x = m_stream.read<uint32>();
				word_y = word_x;
                word_z = word_x;
                word_w = word_x;
                break;
            case UNPACK_S16:
				word_x = m_stream.read<uint16>();
                word_y = word_x;
                word_z = word_x;
                word_w = word_x;
                break;
            case UNPACK_S8:
				word_x = m_stream.read<uint8>();
                word_y = word_x;
                word_z = word_x;
                word_w = word_x;
                break;
            case UNPACK_V232:
				word_x = m_stream.read<uint32>();
				word_y = m_stream.read<uint32>();
                break;
            case UNPACK_V216:
				word_x = m_stream.read<uint16>();
				word_y = m_stream.read<uint16>();
                break;
            case UNPACK_V28:
				word_x = m_stream.read<uint8>();
				word_y = m_stream.read<uint8>();
                break;
            case UNPACK_V332:
				word_x = m_stream.read<uint32>();
				word_y = m_stream.read<uint32>();
				word_z = m_stream.read<uint32>();
                break;
            case UNPACK_V316:
				word_x = m_stream.read<uint16>();
				word_y = m_stream.read<uint16>();
				word_z = m_stream.read<uint16>();
                break;
            case UNPACK_V38:
				word_x = m_stream.read<uint8>();
				word_y = m_stream.read<uint8>();
				word_z = m_stream.read<uint8>();
                break;
            case UNPACK_V432:
				word_x = m_stream.read<uint32>();
				word_y = m_stream.read<uint32>();
				word_z = m_stream.read<uint32>();
				word_w = m_stream.read<uint32>();
				break;
            case UNPACK_V416:
				word_x = m_stream.read<uint16>();
				word_y = m_stream.read<uint16>();
				word_z = m_stream.read<uint16>();
				word_w = m_stream.read<uint16>();
				break;
            case UNPACK_V48:
				word_x = m_stream.read<uint8>();
				word_y = m_stream.read<uint8>();
				word_z = m_stream.read<uint8>();
				word_w = m_stream.read<uint8>();
				break;
            case UNPACK_V45:
                break;
            default:
                break;
        }

        if ( rVIF1_MODE == MODE_ADD ) {
            if ( _usn == 0 ) {
                word_x = (int)word_x+(int)rVIF1_R0;
                word_y = (int)word_y+(int)rVIF1_R1;
                word_z = (int)word_z+(int)rVIF1_R2;
                word_w = (int)word_w+(int)rVIF1_R3;
            } else {
                word_x = word_x+rVIF1_R0;
                word_y = word_y+rVIF1_R1;
                word_z = word_z+rVIF1_R2;
                word_w = word_w+rVIF1_R3;
            }
        } else if ( rVIF1_MODE == MODE_ADDROW ) {
            if ( _usn == 0 ) {
                word_x = (int)word_x+(int)rVIF1_R0;
                word_y = (int)word_y+(int)rVIF1_R1;
                word_z = (int)word_z+(int)rVIF1_R2;
                word_w = (int)word_w+(int)rVIF1_R3;
            } else {
                word_x = word_x+rVIF1_R0;
                word_y = word_y+rVIF1_R1;
                word_z = word_z+rVIF1_R2;
                word_w = word_w+rVIF1_R3;
            }
            rVIF1_R0 = word_x;
            rVIF1_R1 = word_y;
            rVIF1_R2 = word_z;
            rVIF1_R3 = word_w;
        }
        m_pVu->WriteMemX(m_memIndex, word_x);
        m_pVu->WriteMemY(m_memIndex, word_y);
        m_pVu->WriteMemZ(m_memIndex, word_z);
        m_pVu->WriteMemW(m_memIndex, word_w);
        m_memIndex++;
        cycle++;
        write++;
    }
    return E_OK;
}

const int32
Vif::CmdMscal(const int32& data) {
    m_vifCmd = VIF_MSCAL;
    _addr = data&0xffff;
    if ( m_trace ) {
        m_pLog->Trace(1, Log::Format("MSCAL at %d\n", _addr));
    }
    m_pVu->Run(_addr);
    return E_OK;
}

const int32
Vif::CmdMscalf(const int32& data) {
    // TODO simulate the wait for GIF PATH1/PATH2 and
    // end of microprogram ( ie mpg is running while a new batch is waiting to
    // be transfered from VIF to VU

    m_vifCmd = VIF_MSCALF;
    _addr = data&0xffff;
    if ( m_trace ) {
        m_pLog->Trace(1, Log::Format("MSCALF at %d\n", _addr));
    }
    m_pVu->Run(_addr);

    m_state = VIF_STATE_STOPPED;
    return E_OK;
}

const int32
Vif::CmdMscnt(void) {
    if ( m_trace ) {
        m_pLog->Trace(1, "MSCNT\n");
    }
    m_pVu->Run();
    return E_OK;
}

const int32
Vif::CmdStmask(void) {
	uint32 data = m_stream.read<uint32>();
	
	m_vifCmd = VIF_STMASK;
    rVIF0_MASK = data;
    rVIF1_MASK = data;
    if ( m_trace ) {
        m_pLog->Trace(1, "STMASK\n");
    }
    return E_OK;
}

const int32
Vif::CmdStrow(void) {
    if ( m_trace ) {
        m_pLog->Trace(1, "STROW\n");
    }
	uint32 data = m_stream.read<uint32>();
	
	rVIF0_R0 = data;
    rVIF1_R0 = data;

	data = m_stream.read<uint32>();
	
	rVIF0_R1 = data;
    rVIF1_R1 = data;

	data = m_stream.read<uint32>();
	
	rVIF0_R2 = data;
    rVIF1_R2 = data;

	data = m_stream.read<uint32>();
	
	rVIF0_R3 = data;
    rVIF1_R3 = data;
    return E_OK;
}

const int32
Vif::CmdStcol(void) {

	int32 data = m_stream.read<int32>();

	rVIF0_C0 = data;
    rVIF1_C0 = data;

	data = m_stream.read<int32>();
	
	rVIF0_C1 = data;
    rVIF1_C1 = data;

	data = m_stream.read<int32>();

	rVIF0_C2 = data;
    rVIF1_C2 = data;

	data = m_stream.read<int32>();
	
	rVIF0_C3 = data;
    rVIF1_C3 = data;
    return E_OK;
}

// --------------------------------------------------------------------------
// vif0/vif1 common register to text functions
std::vector<std::string>
Vif::UnpackR(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("R");
    sprintf(val, "0x%x", REGISTERS[reg]);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif::UnpackC(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("C");
    sprintf(val, "0x%x", REGISTERS[reg]);
    v.push_back(val);
    return v;
}

std::vector<std::string>
Vif::UnpackItops(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("ITOPS");
    sprintf(val, "0x%x", REGISTERS[reg]&0x3FF);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif::UnpackItop(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("ITOP");
    sprintf(val, "0x%x", REGISTERS[reg]&0x3FF);
    v.push_back(val);
    return v;
}

std::vector<std::string>
Vif::UnpackCycle(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("CL");
    sprintf(val, "0x%x", REGISTERS[reg]&0xFF);
    v.push_back(val);
    v.push_back("WL");
    sprintf(val, "0x%x", (REGISTERS[reg]&0xFF00)>>8);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif::UnpackMode(const int reg) {
    std::vector<std::string> v;
    v.push_back("MODE");
    v.push_back(tMOD[REGISTERS[reg]&0x3]);
    return v;
}
std::vector<std::string>
Vif::UnpackErr(const int reg) {
    std::vector<std::string> v;
    v.push_back("MII");
    v.push_back(tMII[REGISTERS[reg]&0x1]);
    v.push_back("ME0");
    v.push_back(tME0[(REGISTERS[reg]&0x2)>>1]);
    v.push_back("ME1");
    v.push_back(tME1[(REGISTERS[reg]&0x4)>>2]);
    return v;
}
std::vector<std::string>
Vif::UnpackMark(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("NUM");
    sprintf(val, "0x%x", REGISTERS[reg]&0x000000ff);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif::UnpackNum(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("NUM");
    sprintf(val, "0x%x", REGISTERS[reg]&0x000000ff);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif::UnpackMask(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("NUM");
    sprintf(val, "0x%x", REGISTERS[reg]&0x000000ff);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif::UnpackCode(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("IMMEDIATE");
    sprintf(val, "0x%x", REGISTERS[reg]&0x0000ffff);
    v.push_back(val);
    v.push_back("NUM");
    sprintf(val, "0x%x", (REGISTERS[reg]&0x00ff0000)>>16);
    v.push_back(val);
    v.push_back("CMD");
    sprintf(val, "0x%x", (REGISTERS[reg]&0xff000000)>>24);
    v.push_back(val);
    return v;
}

void
Vif::writeRegister(void) {
}

}
