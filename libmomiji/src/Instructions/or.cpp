#include <Decoder.h>
#include <System.h>

#include "./Utils.h"

#include "or.h"

namespace momiji::instr
{
    momiji::System or_instr(momiji::System& sys, const InstructionData& data)
    {
        auto& pc          = sys.cpu.programCounter;
        const auto srcval = utils::readOperandVal(sys, data, 0);

        switch (data.size)
        {
        case 1:
        {
            auto dstval = utils::readOperandPtr<std::int8_t>(sys, data, 1);
            *dstval     = *dstval | (srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dstval = utils::readOperandPtr<std::int16_t>(sys, data, 1);
            *dstval     = *dstval | (srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dstval = utils::readOperandPtr<std::int32_t>(sys, data, 1);
            *dstval     = *dstval | srcval;
        }
        break;
        }

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(data, 0));
        pc += std::uint8_t(utils::isImmediate(data, 1));

        return sys;
    }

    momiji::System ori(momiji::System& sys, const InstructionData& data)
    {
        return or_instr(sys, data);
    }
} // namespace momiji::instr
