#include "div.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{

    momiji::System divs(momiji::System& sys, const InstructionData& data)
    {
        std::int32_t srcval = utils::readOperand1(sys, data);
        srcval              = utils::sign_extend<std::int16_t>(srcval);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.mod2);

        std::int32_t dstval = sys.cpu.dataRegisters[dstreg].value;
        std::int32_t quot   = (dstval / srcval) & 0x0000'FFFF;
        std::int32_t rem    = (dstval % srcval) & 0x0000'FFFF;

        sys.cpu.dataRegisters[dstreg].value = (rem << 16) | (quot);

        return sys;
    }

    momiji::System divu(momiji::System& sys, const InstructionData& data)
    {

        std::int32_t srcval = utils::readOperand1(sys, data);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.mod2);

        std::int32_t dstval = sys.cpu.dataRegisters[dstreg].value;
        std::int32_t quot   = (dstval / srcval) & 0x0000'FFFF;
        std::int32_t rem    = (dstval % srcval) & 0x0000'FFFF;

        sys.cpu.dataRegisters[dstreg].value = (rem << 16) | (quot);

        return sys;
    }
} // namespace momiji::instr
