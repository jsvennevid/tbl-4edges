#ifndef _FIFO
#define _FIFO
#include <vector>
#include "datatypes.h"
#include "sub.h"

namespace ps2emu
{

enum FIFO_REGISTERS {
    VIF1_FIFO, IPU_out_FIFO
};
static const char *tFIFO_REGISTERS[] = {
	"VIF1_FIFO", "IPU_out_FIFO"
};

class FIFO : public SubSystem {
public:
    FIFO();
	~FIFO();
    static const int    nREGISTERS;
    std::vector<std::string>      getRegisterText(const int reg);
private:
    std::vector<std::string>      unpack_VIF1_FIFO(const int reg);
    std::vector<std::string>      unpack_IPU_out_FIFO(const int reg);
};
#endif

}
