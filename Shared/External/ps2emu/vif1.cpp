// -*- C++ -*-
// (C) 2004 by Khaled Daham, <khaled@w-arts.com>

#include <vector>
#include <string>
#include <fstream>

//#include "wx/std::string.h"

#include "vif.h"
#include "vif1.h"
#include "Log.h"
#include "Callbacks.h"

namespace ps2emu
{

const int Vif1::nREGISTERS = 22;

static const std::string tVPS[4] = {
    "Idle",
    "Waits for the data following VIFcode.",
    "Decoding the VIFcode.",
    "Decompressing/transfering the data following VIFcode."
};
static const std::string tVEW[2] = {
    "Not-wait",
    "Wait (VU0 is executing a microprogram)"
};
static const std::string tVGW[2] = {
    "Not-wait",
    "Wait (Stopped status with one of FLUSH/FLUSHA, DIRECT/DIRECTHL)"
};
static const std::string tMRK[2] = {
    "Not-detect",
    "Detect"
};
static const std::string tDBF[2] = {
    "TOPS = BASE",
    "TOPS = BASE+OFFSET"
};
static const std::string tVSS[2] = {
    "Not-stall",
    "Stall"
};
static const std::string tVFS[2] = {
    "Not-stall",
    "Stall"
};
static const std::string tVIS[2] = {
    "Not-stall",
    "Stall"
};
static const std::string tINT[2] = {
    "Not-detect",
    "Detect"
};
static const std::string tER0[2] = {
    "No error",
    "Error (DMAtag has been detected in VIF packet)"
};
static const std::string tER1[2] = {
    "Not-detect ",
    "Detect (Undefined VIFcode has been detected)"
};
static const std::string tFDR[2] = {
    "Main memory/SPRAM -> VIF1",
    "VIF1 -> Main memory/SPRAM"
};

/////////////////////////////// PUBLIC ///////////////////////////////////////

Vif1::Vif1() : Vif(Vif1::nREGISTERS) {
}

Vif1::Vif1(Parser* parser, Vu* vu) : Vif(Vif1::nREGISTERS) {
    m_pParser = parser;
    m_pVu = vu;
	m_trace = true;
	Init();
}

Vif1::~Vif1() {
}

const std::vector<std::string>
Vif1::GetRegisterText(const int reg) {
    switch(reg) {
        case _VIF1_STAT:
            return UnpackStat(reg);
            break;
        // case _VIF1_FBRST:
            // return Unpack__VIF1_FBRST(reg);
            // break;
        case _VIF1_ERR:
            return UnpackErr(reg);
            break;
        case _VIF1_MARK:
            return UnpackMark(reg);
            break;
        case _VIF1_CYCLE:
            return UnpackCycle(reg);
            break;
        case _VIF1_MODE:
            return UnpackMode(reg);
            break;
        case _VIF1_NUM:
            return UnpackNum(reg);
            break;
        case _VIF1_MASK:
            return UnpackMask(reg);
            break;
        case _VIF1_CODE:
            return UnpackCode(reg);
            break;
        case _VIF1_ITOPS:
            return UnpackItops(reg);
            break;
        case _VIF1_ITOP:
            return UnpackItop(reg);
            break;
        case _VIF1_BASE:
            return UnpackBase(reg);
            break;
        case _VIF1_OFST:
            return UnpackOfst(reg);
            break;
        case _VIF1_TOP:
            return UnpackTop(reg);
            break;
        case _VIF1_TOPS:
            return UnpackTops(reg);
            break;
        case _VIF1_R0:
        case _VIF1_R1:
        case _VIF1_R2:
        case _VIF1_R3:
            return UnpackR(reg);
            break;
        case _VIF1_C0:
        case _VIF1_C1:
        case _VIF1_C2:
        case _VIF1_C3:
            return UnpackC(reg);
            break;
    }
    std::vector<std::string> v;
    return v;
}

const int32 
Vif1::Read() {
/*
	int32 ret;
    Init();
    if ( m_trace ) {
        m_pLog->Trace(0, "VIF\n");
    }
    while(m_fin->peek() != EOF) {
        ret = DecodeCmd();
        if ( ret != E_OK ) {
            m_pLog->Warning("VIF Decoder failed\n");
            return ret;
        }
    }
    return E_OK;
*/
	return 0;
}

const int32 Vif1::Read(const u32* data, const uint16 size)
{
	m_pLog->Trace(0, "VIF\n");

	m_stream.setBuffer(data, size * 4);

	while (!m_stream.isFinished())
	{
		u32 ret = DecodeCmd();

		if (ret != E_OK)
		{
            m_pLog->Warning("VIF Decoder failed\n");
			return ret;
		}
	}

	return 0;
}
/*
const int32 
Vif1::Read(std::ifstream* fin, const uint16 numQuad) {
    int32 ret;
    uint32 m_start;
    uint16 m_delta = 0;
    m_fin = fin;
    m_start = m_fin->tellg();
    if ( m_trace ) {
        m_pLog->Trace(0, "VIF\n");
    }
    while(m_delta < (numQuad*16)) {
        ret = DecodeCmd();
        if ( ret != E_OK ) {
            m_pLog->Warning("VIF Decoder failed\n");
            return ret;
        }
        m_delta = ((int)m_fin->tellg())-m_start;
    }
    return 0;
}
*/

/////////////////////////////// PRIVATE ///////////////////////////////////////
const int32
Vif1::DecodeCmd(void) 
{
    //m_fin->read((char*)&raw, 4);
    //data = ((uint32)raw[3]<<24) + ((uint32)raw[2]<<16) + ((uint32)raw[1]<<8) + (uint32)raw[0];

	uint32 data = m_stream.read<uint32>();
	
	//if (m_stream.isFinished()) {
    //    return E_FILE_EOF;
    //}
	uint32 cmd = data>>24;
    uint32 vl, vn;
    switch(cmd) {
        case VIF_NOP:
            CmdNop();
            break;
        case VIF_STCYCL:
            CmdStcycl(data);
            break;
        case VIF_OFFSET:
            CmdOffset(data);
            break;
        case VIF_BASE:
            CmdBase(data);
            break;
        case VIF_ITOP:
            CmdItop(data);
            break;
        case VIF_STMOD:
            CmdStmod(data);
            break;
        case VIF_MSKPATH3:
            CmdMaskpath3(data);
            break;
        case VIF_MARK:
            CmdMark(data);
            break;
        case VIF_FLUSHE:
            CmdFlushE();
            break;
        case VIF_FLUSH:
            CmdFlush();
            break;
        case VIF_FLUSHA:
            CmdFlushA();
            break;
        case VIF_MSCAL:
            CmdMscal(data);
            break;
        case VIF_MSCNT:
            CmdMscnt();
            break;
        case VIF_MSCALF:
            CmdMscalf(data);
            break;
        case VIF_STMASK:
            CmdStmask();
            break;
        case VIF_STROW:
            CmdStrow();
            break;
        case VIF_STCOL:
            CmdStcol();
            break;
        case VIF_MPG:
            CmdMpg(data);
            break;
        case VIF_DIRECT:
            CmdDirect(data);
            break;
        case VIF_DIRECTHL:
            CmdDirectHl(data);
            break;
        default:
            if ( (cmd&VIF_UNPACK) == VIF_UNPACK) {
                if ( m_trace ) {
                    m_pLog->Trace(1, "UNPACK\n");
                }
                m_memIndex = 0;
                m_vifCmd = VIF_UNPACK;
                m_vifCmdNum = (data>>16)&0xFF;
                if ( m_vifCmdNum == 0 ) {
                    m_vifCmdNum = 256;
                }
                m_unpack = (data>>24)&0xF;
                _addr = (data)&0x3FF;
                _usn = (data>>14)&0x1;
                _flg = (data>>15)&0x1;
                _mask = (bool)((data>>27)&0x1);
                vl = (data>>24)&3;
                vn = (data>>26)&3;

                if ( _WL <= _CL ) {
                    m_length = 1+(((32>>vl)*(vn+1))*m_vifCmdNum/32);
                } else {
                    uint32 n = _CL*(m_vifCmdNum/_WL)+limit(m_vifCmdNum%_WL,_CL);
                    m_length = 1+(((32>>vl)*(vn+1))*n/32);
                }

                m_unpack = (vn<<2)+vl;
                if ( _flg == 1 ) {
                    m_memIndex = rVIF1_TOPS; 
                }
                m_memIndex += _addr;
                return CmdUnpack();
            } else {
                m_pLog->Warning("Illegal VIF code\n");
                return E_VIF_DECODE;
            }
    }
    return E_OK;
}

const int32
Vif1::CmdOffset(const int32& data) {
    rVIF1_OFST = data&0x3FF;
    // rVIF_STAT::DBF = 0
    rVIF1_STAT = (rVIF1_STAT&0x1f803f4f);
    rVIF1_TOPS = rVIF1_BASE;
    if ( m_trace ) {
        m_pLog->Trace(1, Log::Format("OFFSET = %d\n", rVIF1_OFST));
    }
    return E_OK;
}

const int32
Vif1::CmdBase(const int32& data) {
    rVIF1_BASE = data&0x3FF;
    if ( m_trace ) {
        m_pLog->Trace(1, Log::Format("BASE = %d\n", rVIF1_BASE));
    }
    return E_OK;
}

const int32
Vif1::CmdMaskpath3(const int32& data) {
    // TODO
    // enables/disables transfer via PATH3
    m_vifCmd = VIF_MSKPATH3;
    m_maskPath3 = (bool)((data&0x8000)>>15);
    if ( m_trace ) {
        m_pLog->Trace(1, Log::Format("MSKPATH3 = %d\n", m_maskPath3));
    }
    return E_OK;
}

const int32
Vif1::CmdFlush(void) {
    // TODO
    // waits for the state in wich transfers to the GIF from PATH1 and PATH2
    // have been ended after end of mpg. ( any xgkick or directh/directhl ).
    if ( m_trace ) {
        m_pLog->Trace(1, "FLUSH\n");
    }
    return E_OK;
}

const int32
Vif1::CmdFlushA(void) {
    // TODO
    // waits for end of mpg and the state where there is no xfer request from
    // PATH3, and end of transfer from PATH1 and PATH2
    if ( m_trace ) {
        m_pLog->Trace(1, "FLUSHA\n");
    }
    return E_OK;
}

const int32
Vif1::CmdDirect(const int32& data) {
    // TODO
    uint32 size = data&0xffff;
    if ( m_trace ) {
        m_pLog->Trace(1, Log::Format("DIRECT, size(QWC) = %d\n", size));
    }
    // read 16KB at a time, dump and gsExec
    uint32 total = 0;
    uint32 len = 16*1024;
    static unsigned char buffer[16*1024];
    while(total < size) {
        if ( size < len ) {
            len = size;
        } else if ((size - total) < len) {
            len = size - total;
        }

		m_stream.readBuffer((char*)&buffer, len * 16);	
        //m_fin->read(reinterpret_cast<char *>(buffer), len);
        //Remote::GsExec(buffer, len);

		//Callbacks::callGif(buffer);

		total += len;
    }
    return E_OK;
}

const int32
Vif1::CmdDirectHl(const int32& data) {
    // TODO
    // DIRECTHL does not interrupt PATH3 IMAGE mode
    uint32 size = data&0xffff;
    if ( m_trace ) {
        m_pLog->Trace(1, Log::Format("DIRECTHL, size(QWC) = %d\n", size));
    }

    // read 16KB at a time, dump and gsExec
    uint32 total = 0;
    uint32 len = 16*1024;
    static unsigned char buffer[16*1024];
    while(total < size) {
        if ( size < len ) {
            len = size;
        } else if ((size - total) < len) {
            len = size - total;
        }

		m_stream.readBuffer((char*)&buffer, len);	
        //m_fin->read(reinterpret_cast<char *>(buffer), len);
        //Remote::GsExec(buffer, len);
        total += len;
    }
    return E_OK;
}

// --------------------------------------------------------------------------
// register to text functions
std::vector<std::string>
Vif1::UnpackStat(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("VPS");
    v.push_back(tVPS[REGISTERS[reg]&0x00000003]);
    v.push_back("VEW");
    v.push_back(tVEW[(REGISTERS[reg]&0x00000004)>>2]);
    v.push_back("VGW");
    v.push_back(tVGW[(REGISTERS[reg]&0x00000008)>>3]);
    v.push_back("MRK");
    v.push_back(tMRK[(REGISTERS[reg]&0x00000040)>>6]);
    v.push_back("DBF");
    v.push_back(tDBF[(REGISTERS[reg]&0x00000080)>>7]);
    v.push_back("VSS");
    v.push_back(tVSS[(REGISTERS[reg]&0x00000100)>>8]);
    v.push_back("VFS");
    v.push_back(tVFS[(REGISTERS[reg]&0x00000200)>>9]);
    v.push_back("VIS");
    v.push_back(tVIS[(REGISTERS[reg]&0x00000400)>>10]);
    v.push_back("INT");
    v.push_back(tINT[(REGISTERS[reg]&0x00000800)>>11]);
    v.push_back("ER0");
    v.push_back(tER0[(REGISTERS[reg]&0x00001000)>>12]);
    v.push_back("ER1");
    v.push_back(tER1[(REGISTERS[reg]&0x00002000)>>13]);
    v.push_back("FDR");
    v.push_back(tFDR[(REGISTERS[reg]&0x00800000)>>23]);
    v.push_back("FQC");
    sprintf(val, "0x%x", (REGISTERS[reg]&0x1F000000)>>24);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif1::UnpackTops(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("TOPS");
    sprintf(val, "%d", REGISTERS[reg]&0x3ff);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif1::UnpackTop(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("TOP");
    sprintf(val, "%d", REGISTERS[reg]&0x3ff);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif1::UnpackBase(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("BASE");
    sprintf(val, "%d", REGISTERS[reg]&0x3ff);
    v.push_back(val);
    return v;
}
std::vector<std::string>
Vif1::UnpackOfst(const int reg) {
    std::vector<std::string> v;
    char val[100];
    v.push_back("OFFSET");
    sprintf(val, "%d", REGISTERS[reg]&0x3ff);
    v.push_back(val);
    return v;
}

}
