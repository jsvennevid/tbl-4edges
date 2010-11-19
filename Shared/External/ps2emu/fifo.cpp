#include <string>
#include <stdio.h>
#include "fifo.h"

namespace ps2emu
{

const int FIFO::nREGISTERS = 2;
std::vector<std::string> g_TemporaryList;

FIFO::FIFO() : SubSystem(FIFO::nREGISTERS) {
}
FIFO::~FIFO() {
}
std::vector<std::string>
FIFO::getRegisterText(const int reg) {
    switch (reg) {
        case VIF1_FIFO:
            return unpack_VIF1_FIFO(reg);
            break;
        case IPU_out_FIFO:
            return unpack_IPU_out_FIFO(reg);
            break;
    }
}

std::vector<std::string>
FIFO::unpack_VIF1_FIFO(const int reg)  {
  return g_TemporaryList;
}

std::vector<std::string>
FIFO::unpack_IPU_out_FIFO(const int reg)  {
  return g_TemporaryList;
}

}
