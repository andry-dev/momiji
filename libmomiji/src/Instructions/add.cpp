#include "add.h"

#include "./Utils.h"
#include <Utils.h>

namespace momiji::instr
{
    momiji::System add(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter;

        const std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        std::int32_t result = 0;

        auto& statusReg = sys.cpu.statusRegister;

        switch (data.size)
        {
        case 1:
        {
            auto dstreg = utils::readOperandPtr<std::int8_t>(sys, data, 1);
            *dstreg     = *dstreg + (srcval & 0x0000'00FF);
            result      = utils::sign_extend<std::int8_t>(*dstreg);
            statusReg.overflow =
                utils::add_overflow(*dstreg, std::int8_t(srcval));
        }
        break;

        case 2:
        {
            auto dstreg = utils::readOperandPtr<std::int16_t>(sys, data, 1);
            *dstreg     = *dstreg + (srcval & 0x0000'FFFF);
            result      = utils::sign_extend<std::int16_t>(*dstreg);
            statusReg.overflow =
                utils::add_overflow(*dstreg, std::int16_t(srcval));
        }
        break;

        case 4:
        {
            auto dstreg = utils::readOperandPtr<std::int32_t>(sys, data, 1);
            *dstreg     = *dstreg + srcval;
            result      = *dstreg;
            statusReg.overflow = utils::add_overflow(*dstreg, srcval);
        }
        break;
        }

        statusReg.negative = result < 0;
        statusReg.zero     = result == 0;

        // How the fuck do I detect a carry
        statusReg.carry  = 0;
        statusReg.extend = statusReg.carry;

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(data, 0));
        pc += std::uint8_t(utils::isImmediate(data, 1));

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
