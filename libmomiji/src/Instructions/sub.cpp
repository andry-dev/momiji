#include "sub.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{

    // This functions will still check for an address register as a
    // destination
    momiji::System sub(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;

        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        switch (data.size)
        {
        case 1:
        {
            auto dst = utils::readOperandPtr8(sys, data, 1);
            *dst     = *dst - (srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dst = utils::readOperandPtr16(sys, data, 1);
            *dst     = *dst - (srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dst = utils::readOperandPtr32(sys, data, 1);
            *dst     = *dst - srcval;
        }
        break;
        }

        pc += 2;
        pc += utils::isImmediate(data, 0);
        pc += utils::isImmediate(data, 1);

        return sys;
    }

    momiji::System suba(momiji::System& sys, const InstructionData& data)
    {
        return instr::sub(sys, data);
    }

    momiji::System subi(momiji::System& sys, const InstructionData& data)
    {
        return instr::sub(sys, data);
    }
} // namespace momiji::instr
