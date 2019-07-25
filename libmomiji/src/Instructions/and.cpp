#include "and.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System and_instr(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;

        const std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        switch (data.size)
        {
        case 1:
        {
            auto dstreg = utils::readOperandPtr8(sys, data, 1);
            *dstreg     = *dstreg & (srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dstreg = utils::readOperandPtr16(sys, data, 1);
            *dstreg     = *dstreg & (srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dstreg = utils::readOperandPtr32(sys, data, 1);
            *dstreg     = *dstreg & srcval;
        }
        break;
        }

        pc += 2;
        pc += utils::isImmediate(data, 0);
        pc += utils::isImmediate(data, 1);

        return sys;
    }

    momiji::System andi(momiji::System& sys, const InstructionData& data)
    {
        return and_instr(sys, data);
    }
} // namespace momiji::instr
