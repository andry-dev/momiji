#include "move.h"

#include "Utils.h"

#include <iostream>

namespace momiji::instr
{
    momiji::System move(momiji::System& sys, const InstructionData& data)
    {
        // For data and address registers the value is already stored
        std::uint32_t srcval = utils::readOperand1(sys, data);
        auto pc = sys.cpu.programCounter.address;

        switch (data.size)
        {
        case 1:
        {
            auto dst = utils::readDestOp8(sys, data);
            *dst = (srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dst = utils::readDestOp16(sys, data);
            *dst = (srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dst = utils::readDestOp32(sys, data);
            *dst = srcval;
        }
        break;
        }

        return sys;
    }
} // namespace momiji::instr
