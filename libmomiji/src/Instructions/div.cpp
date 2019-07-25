#include "div.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{

    momiji::System divs(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;

        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        srcval = utils::sign_extend<std::int16_t>(srcval);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.addressingMode[1]);

        std::int32_t dstval = sys.cpu.dataRegisters[dstreg].value;
        std::int32_t quot   = (dstval / srcval) & 0x0000'FFFF;
        std::int32_t rem    = (dstval % srcval) & 0x0000'FFFF;

        sys.cpu.dataRegisters[dstreg].value = (rem << 16) | (quot);

        pc += 2;
        pc += utils::isImmediate(data, 0);

        return sys;
    }

    momiji::System divu(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;

        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.addressingMode[1]);

        std::int32_t dstval = sys.cpu.dataRegisters[dstreg].value;
        std::int32_t quot   = (dstval / srcval) & 0x0000'FFFF;
        std::int32_t rem    = (dstval % srcval) & 0x0000'FFFF;

        sys.cpu.dataRegisters[dstreg].value = (rem << 16) | (quot);

        pc += 2;
        pc += utils::isImmediate(data, 0);

        return sys;
    }
} // namespace momiji::instr
