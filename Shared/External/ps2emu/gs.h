#ifndef _GS
#define _GS
#include <vector>
#include "datatypes.h"
#include "sub.h"

namespace ps2emu
{

enum GS_REGISTERS {
    GS_SIGLBLD, GS_CSR
};
static const char *tGS_REGISTERS[] = {
	"SIGLBLD", "CSR"
};

class GS : public SubSystem {
public:
    GS();
    ~GS();
    static const int    nREGISTERS;
    std::vector<std::string>      getRegisterText(const int reg);
private:
    std::vector<std::string>      unpack_SIGLBLID(const int reg);
    std::vector<std::string>      unpack_CSR(const int reg);

};

}

#endif
