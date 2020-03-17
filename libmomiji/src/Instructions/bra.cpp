#include "bra.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System bra(momiji::System& sys, const InstructionData& data)
    {
        std::int16_t offset = utils::to_val(data.operandType[0]);

        auto& pc = sys.cpu.programCounter;

        auto memview = momiji::make_memory_view(sys);

        if (offset == 0)
        {
            const auto nextloc = pc + 2;

            offset = std::int16_t(*memview.read16(nextloc.raw()));
        }

        auto& signed_pc = *pc.as<std::int32_t>();
        signed_pc += std::int32_t(offset);

        return sys;
    }

    momiji::System bsr(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter;
        auto& sp = sys.cpu.addressRegisters[7];

        sp -= 4;

        const auto writeRes = sys.mem.write32(pc.raw(), sp.raw());

        if (!writeRes)
        {
            // Do something with this
        }

        std::int16_t offset = utils::to_val(data.operandType[0]);

        if (offset == 0)
        {
            const auto nextloc = pc + 2;

            offset = std::int16_t(*sys.mem.read16(nextloc.raw()));
        }

        auto& signed_pc = *pc.as<std::int32_t>();

        signed_pc += std::int32_t(offset);

        return sys;
    }
} // namespace momiji::instr
