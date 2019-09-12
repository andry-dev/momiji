#include "sub.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{

    // This functions will still check for an address register as a
    // destination
    momiji::System sub(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;

        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        auto& statusReg = sys.cpu.statusRegister;

        switch (data.size)
        {
        case 1:
        {
            auto dst = utils::readOperandPtr<std::int8_t>(sys, data, 1);
            *dst     = *dst - std::int8_t(srcval & 0x0000'00FF);
        }
        break;

        case 2:
        {
            auto dst = utils::readOperandPtr<std::int16_t>(sys, data, 1);
            *dst     = *dst - std::int8_t(srcval & 0x0000'FFFF);
        }
        break;

        case 4:
        {
            auto dst = utils::readOperandPtr<std::int32_t>(sys, data, 1);
            *dst     = *dst - srcval;
        }
        break;
        }

        statusReg.carry  = 0;
        statusReg.extend = statusReg.carry;

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(data, 0));
        pc += std::uint8_t(utils::isImmediate(data, 1));

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
