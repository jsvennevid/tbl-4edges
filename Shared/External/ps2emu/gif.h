#ifndef _GIF
#define _GIF
#include "datatypes.h"
#include "sub.h"

namespace ps2emu
{

static const char *tGIF_REGISTERS[] = {
    "GIF STAT", "GIF TAG0", "GIF TAG1",
    "GIF TAG2", "GIF TAG3", "GIF CNT",
    "GIF P3CNT", "GIF P3TAG"
};

enum GIF_REGISTERS {
    GIF_STAT, GIF_TAG0, GIF_TAG1, GIF_TAG2, GIF_TAG3,
    GIF_CNT, GIF_P3CNT, GIF_P3TAG
};

class Gif : public SubSystem {
public:
    Gif();
    Gif(uint32 *, uint32);
    void            Init(void);
    std::string          GetReadableTag();
    uint32          GetNloop();
    uint32          GetNreg();
    uint32          GetRegs();
    std::string          RegsAsString();
    std::vector<std::string>  TagAsString();
    std::vector<std::string>  NloopData();

    const std::vector<std::string>    GetRegisterText(const int reg);
    void            SetRegisters(uint32 *data, uint32 size);

    bool            Unpack();
    // variables
    long            xoffset;
    long            yoffset;
    static int const nREGISTERS;
private:
    // functions
    void            UnpackNloop(void);
    void            UnpackNreg(void);
    void            UnpackFlag(void);
    void            UnpackPrim(void);
    void            UnpackPre(void);
    void            UnpackEop(void);
    void            UnpackRegisters(void);
    void            ParsePrim(std::vector<std::string> &);
    bool            Validate(void);

    // EE mapped register functions
    std::vector<std::string>  UnpackStat(const int reg); 
    std::vector<std::string>  UnpackTag0(const int reg); 
    std::vector<std::string>  UnpackTag1(const int reg); 
    std::vector<std::string>  UnpackTag2(const int reg); 
    std::vector<std::string>  UnpackTag3(const int reg); 
    std::vector<std::string>  UnpackCnt(const int reg); 
    std::vector<std::string>  UnpackP3cnt(const int reg); 
    std::vector<std::string>  UnpackP3tag(const int reg); 

    // variables
    unsigned char   registers[16];
    uint32          nloop;
    uint32          eop;
    uint32          nreg;
    uint32          flag;
    uint32          prim;
    uint32          pre;
    ivec            gifData[1024];

    uint32          counter;
    uint32          curNreg;
    uint32          curNloop;
};

int const GIFnREGISTERS = 8;

}

#endif
