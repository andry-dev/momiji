#include "mul.h"

#include <Utils.h>

namespace momiji::instr
{

    momiji::System muls(momiji::System sys, const InstructionData& data)
    {
        auto* pc = sys.cpu.programCounter.address;

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.mod2);

        std::int32_t srcval = utils::to_val(data.mod1);
        switch (data.op1)
        {
        case OperandType::DataRegister:
            srcval = sys.cpu.dataRegisters[srcval].value;
            break;

        case OperandType::AddressRegister:
            srcval = sys.cpu.addressRegisters[srcval].value;
            break;

        case OperandType::Immediate:
            switch (data.mod1)
            {
            case SpecialAddressingMode::Immediate:
                srcval = *(pc + 1);
                break;
            }
            break;
        }

        srcval = utils::sign_extend<std::int16_t>(srcval);

        const std::int32_t dstval =
            utils::sign_extend<std::int16_t>(sys.cpu.dataRegisters[dstreg].value);

        const std::int32_t res = (dstval * srcval);

        sys.cpu.dataRegisters[dstreg].value = res;

        return sys;
    }

    momiji::System mulu(momiji::System sys, const InstructionData& data)
    {
        auto* pc = sys.cpu.programCounter.address;

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.mod2);

        std::int32_t srcval = utils::to_val(data.mod1);
        switch (data.op1)
        {
        case OperandType::DataRegister:
            srcval = sys.cpu.dataRegisters[srcval].value;
            break;

        case OperandType::AddressRegister:
            srcval = sys.cpu.addressRegisters[srcval].value;
            break;

        case OperandType::Immediate:
            switch (data.mod1)
            {
            case SpecialAddressingMode::Immediate:
                srcval = *(pc + 1);
                break;
            }
            break;
        }

        std::int32_t dstval = sys.cpu.dataRegisters[dstreg].value;
        sys.cpu.dataRegisters[dstreg].value = dstval * srcval;

        return sys;
    }
}
