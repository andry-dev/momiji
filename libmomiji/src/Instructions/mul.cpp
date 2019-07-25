#include "mul.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{

    momiji::System muls(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;

        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        srcval = utils::sign_extend<std::int16_t>(srcval);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.addressingMode[1]);

        const std::int32_t dstval = utils::sign_extend<std::int16_t>(
            sys.cpu.dataRegisters[dstreg].value);

        const std::int32_t res = (dstval * srcval);

        sys.cpu.dataRegisters[dstreg].value = res;

        pc += 2;
        pc += utils::isImmediate(data, 0);
        pc += utils::isImmediate(data, 1);

        return sys;
    }

    momiji::System mulu(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;

        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.addressingMode[1]);

        std::int32_t dstval = sys.cpu.dataRegisters[dstreg].value;
        sys.cpu.dataRegisters[dstreg].value = dstval * srcval;

        pc += 2;
        pc += utils::isImmediate(data, 0);
        pc += utils::isImmediate(data, 1);

        return sys;
    }
} // namespace momiji::instr
