#include "add.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System add(momiji::System& sys, const InstructionData& data)
    {
        const std::int32_t srcval = utils::readOperand1(sys, data);

        switch (data.size)
        {
        case 1:
        {
            auto dstreg = utils::readDestOp8(sys, data);
            *dstreg     = *dstreg + (srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dstreg = utils::readDestOp16(sys, data);
            *dstreg     = *dstreg + (srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dstreg = utils::readDestOp32(sys, data);
            *dstreg     = *dstreg + srcval;
        }
        break;
        }

        return sys;
    }

    momiji::System adda(momiji::System& sys, const InstructionData& data)
    {

        return instr::add(sys, data);
    }

    momiji::System addi(momiji::System& sys, const InstructionData& data)
    {
        return instr::add(sys, data);
    }
} // namespace momiji::instr
