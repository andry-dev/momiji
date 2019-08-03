#include "tst.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System tst(momiji::System& sys, const InstructionData& instr)
    {
        auto& pc         = sys.cpu.programCounter.address;
        std::int32_t val = utils::readOperandVal(sys, instr, 0);

        sys.cpu.statusRegister.negative = (val < 0) ? 0b1 : 0b0;
        sys.cpu.statusRegister.zero     = (val == 0) ? 0b1 : 0b0;
        sys.cpu.statusRegister.overflow = 0b0;
        sys.cpu.statusRegister.carry    = 0b0;

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(instr, 0));

        return sys;
    }
} // namespace momiji::instr
