#include "sub.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{

    // This functions will still check for an address register as a
    // destination
    momiji::System sub(momiji::System& sys, const InstructionData& data)
    {
        std::int32_t srcval = utils::readOperand1(sys, data);

        switch (data.size)
        {
        case 1:
        {
            auto dst = utils::readDestOp8(sys, data);
            *dst = *dst - (srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dst = utils::readDestOp16(sys, data);
            *dst = *dst - (srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dst = utils::readDestOp32(sys, data);
            *dst = *dst - srcval;
        }
        break;
        }

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
