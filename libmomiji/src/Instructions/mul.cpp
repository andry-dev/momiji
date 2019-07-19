#include "mul.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{

    momiji::System muls(momiji::System& sys, const InstructionData& data)
    {

        std::int32_t srcval = utils::readOperand1(sys, data);

        srcval = utils::sign_extend<std::int16_t>(srcval);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.mod2);

        const std::int32_t dstval = utils::sign_extend<std::int16_t>(
            sys.cpu.dataRegisters[dstreg].value);

        const std::int32_t res = (dstval * srcval);

        sys.cpu.dataRegisters[dstreg].value = res;

        return sys;
    }

    momiji::System mulu(momiji::System& sys, const InstructionData& data)
    {
        std::int32_t srcval = utils::readOperand1(sys, data);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.mod2);

        std::int32_t dstval = sys.cpu.dataRegisters[dstreg].value;
        sys.cpu.dataRegisters[dstreg].value = dstval * srcval;

        return sys;
    }
} // namespace momiji::instr
