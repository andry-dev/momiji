#include "move.h"

#include "Utils.h"

#include <iostream>

namespace momiji::instr
{
    momiji::System move(momiji::System& sys, const InstructionData& data)
    {
        // For data and address registers the value is already stored
        std::uint32_t srcval = utils::readOperandVal(sys, data, 0);

        auto& pc = sys.cpu.programCounter.address;

        switch (data.size)
        {
        case 1:
        {
            auto dst = utils::readOperandPtr8(sys, data, 1);
            *dst     = (srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dst = utils::readOperandPtr16(sys, data, 1);
            *dst     = (srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dst = utils::readOperandPtr32(sys, data, 1);
            *dst     = srcval;
        }
        break;
        }

        pc += 2;

        pc += (utils::isImmediate(data, 0));
        pc += (utils::isImmediate(data, 1));

        return sys;
    }
} // namespace momiji::instr
