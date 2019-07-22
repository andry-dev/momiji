#include "bra.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System bra(momiji::System& sys, const InstructionData& data)
    {
        std::int16_t offset = utils::to_val(data.op1);

        auto& pc = sys.cpu.programCounter.address;

        auto memview = momiji::make_memory_view(sys);

        if (offset == 0)
        {
            offset = memview.read16(pc + 2);
        }

        pc += offset;

        return sys;
    }

    momiji::System bsr(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;
        auto& sp = sys.cpu.addressRegisters[7].value;

        sp -= 4;

        sys.mem.write32(pc, sp);

        std::int16_t offset = utils::to_val(data.op1);

        if (offset == 0)
        {
            offset = sys.mem.read16(pc + 2);
        }

        pc += offset;

        return sys;
    }
} // namespace momiji::instr
