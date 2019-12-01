#include "div.h"

#include <Utils.h>

#include "./Utils.h"

namespace momiji::instr
{

    momiji::System divs(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter;

        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        srcval = utils::sign_extend<std::int16_t>(srcval);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.addressingMode[1]);

        const std::int32_t dstval =
            asl::saccess(sys.cpu.dataRegisters, dstreg).raw();
        const std::int32_t quot = (dstval / srcval) & 0x0000'FFFF;
        const std::int32_t rem  = (dstval % srcval) & 0x0000'FFFF;

        asl::saccess(sys.cpu.dataRegisters, dstreg) = (rem << 16) | (quot);

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(data, 0));

        return sys;
    }

    momiji::System divu(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter;

        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.addressingMode[1]);

        std::int32_t dstval = asl::saccess(sys.cpu.dataRegisters, dstreg).raw();
        std::int32_t quot   = (dstval / srcval) & 0x0000'FFFF;
        std::int32_t rem    = (dstval % srcval) & 0x0000'FFFF;

        asl::saccess(sys.cpu.dataRegisters, dstreg) = (rem << 16) | (quot);

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(data, 0));

        return sys;
    }
} // namespace momiji::instr
