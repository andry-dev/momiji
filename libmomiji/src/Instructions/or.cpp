#include <Decoder.h>
#include <System.h>

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System or_instr(momiji::System& sys, const InstructionData& data)
    {
        const auto srcval = utils::readOperand1(sys, data);

        switch (data.size)
        {
        case 1:
        {
            auto dstval = utils::readDestOp8(sys, data);
            *dstval     = *dstval | (srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dstval = utils::readDestOp16(sys, data);
            *dstval     = *dstval | (srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dstval = utils::readDestOp32(sys, data);
            *dstval     = *dstval | srcval;
        }
        break;
        }

        return sys;
    }

    momiji::System ori(momiji::System& sys, const InstructionData& data)
    {
        return or_instr(sys, data);
    }
} // namespace momiji::instr
