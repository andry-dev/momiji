#include "rts.h"

namespace momiji::instr
{
    momiji::System rts(momiji::System& sys,
                       const momiji::InstructionData& instr)
    {
        auto& sp = sys.cpu.addressRegisters[7].value;
        auto& pc = sys.cpu.programCounter.address;

        pc = sys.mem.read32(sp);

        sp += 4;

        return sys;
    }
} // namespace momiji::instr
