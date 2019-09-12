#include "bra.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System bra(momiji::System& sys, const InstructionData& data)
    {
        std::int16_t offset = utils::to_val(data.operandType[0]);

        auto& pc = (sys.cpu.programCounter.address);

        auto memview = momiji::make_memory_view(sys);

        if (offset == 0)
        {
            offset = std::int16_t(*memview.read16(pc + 2));
        }

        auto& signed_pc = reinterpret_cast<std::int32_t&>(pc);
        signed_pc += std::int32_t(offset);

        return sys;
    }

    momiji::System bsr(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;
        auto& sp = sys.cpu.addressRegisters[7].value;

        sp -= 4;

        const auto writeRes = sys.mem.write32(pc, sp);

        if (!writeRes)
        {
            // Do something with this
        }

        std::int16_t offset = utils::to_val(data.operandType[0]);

        if (offset == 0)
        {
            offset = std::int16_t(*sys.mem.read16(pc + 2));
        }

        auto& signed_pc = reinterpret_cast<std::int32_t&>(pc);

        signed_pc += std::int32_t(offset);

        return sys;
    }
} // namespace momiji::instr
