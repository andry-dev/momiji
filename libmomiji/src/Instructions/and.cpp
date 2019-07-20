#include "and.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System and_instr(momiji::System& sys, const InstructionData& data)
    {
        const std::int32_t srcval = utils::readOperand1(sys, data);

        switch (data.size)
        {
        case 1:
        {
            auto dstreg = utils::readDestOp8(sys, data);
            *dstreg     = *dstreg & (srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dstreg = utils::readDestOp16(sys, data);
            *dstreg     = *dstreg & (srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dstreg = utils::readDestOp32(sys, data);
            *dstreg     = *dstreg & srcval;
        }
        break;
        }

        return sys;
    }

    momiji::System andi(momiji::System& sys, const InstructionData& data)
    {
        return and_instr(sys, data);
    }
} // namespace momiji::instr
