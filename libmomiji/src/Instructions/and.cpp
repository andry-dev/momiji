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
            auto dstreg = utils::readOperandPtr<std::int8_t>(sys, data, 1);
            *dstreg     = *dstreg & std::int8_t(srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dstreg = utils::readOperandPtr<std::int16_t>(sys, data, 1);
            *dstreg     = *dstreg & std::int16_t(srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dstreg = utils::readOperandPtr<std::int32_t>(sys, data, 1);
            *dstreg     = *dstreg & srcval;
        }
        break;
        }

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(data, 0));
        pc += std::uint8_t(utils::isImmediate(data, 1));

        return sys;
    }

    momiji::System andi(momiji::System& sys, const InstructionData& data)
    {
        return and_instr(sys, data);
    }
} // namespace momiji::instr
