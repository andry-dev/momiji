#include "tst.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System tst(momiji::System& sys, const InstructionData& instr)
    {
        std::uint32_t val = utils::readOperand1(sys, instr);
        

        return sys;
    }
}
