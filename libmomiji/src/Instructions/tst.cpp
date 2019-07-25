#include "tst.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System tst(momiji::System& sys, const InstructionData& instr)
    {
        std::uint32_t val = utils::readOperandVal(sys, instr, 0);

        return sys;
    }
} // namespace momiji::instr
