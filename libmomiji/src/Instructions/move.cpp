#include "move.h"

#include "Utils.h"

#include <iostream>

namespace momiji::instr
{
    momiji::System move(momiji::System& sys, const InstructionData& data)
    {
        // For data and address registers the value is already stored
        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        auto& pc = sys.cpu.programCounter.address;

        switch (data.size)
        {
        case 1:
        {
            auto dst = utils::readOperandPtr<std::int8_t>(sys, data, 1);
            *dst     = std::int8_t((srcval & 0x0000'00FF));
        }
        break;

        case 2:
        {
            auto dst = utils::readOperandPtr<std::int16_t>(sys, data, 1);
            *dst     = std::int16_t((srcval & 0x0000'FFFF));
        }
        break;

        case 4:
        {
            auto dst = utils::readOperandPtr<std::int32_t>(sys, data, 1);
            *dst     = srcval;
        }
        break;
        }

        pc += 2;

        pc += std::uint8_t(utils::isImmediate(data, 0));
        pc += std::uint8_t(utils::isImmediate(data, 1));

        return sys;
    }
} // namespace momiji::instr
