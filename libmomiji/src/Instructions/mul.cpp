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
        std::int32_t& dst = asl::saccess(sys.cpu.dataRegisters, dstreg).value;

        dst = utils::sign_extend<std::int16_t>(dst) * srcval;

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(data, 0));
        pc += std::uint8_t(utils::isImmediate(data, 1));

        return sys;
    }

    momiji::System mulu(momiji::System& sys, const InstructionData& data)
    {
        auto& pc = sys.cpu.programCounter.address;

        std::int32_t srcval = utils::readOperandVal(sys, data, 0);

        // Always a data register
        const std::int32_t dstreg = utils::to_val(data.addressingMode[1]);
        std::int32_t& dst = asl::saccess(sys.cpu.dataRegisters, dstreg).value;

        dst = dst * srcval;

        pc += 2;
        pc += std::uint8_t(utils::isImmediate(data, 0));
        pc += std::uint8_t(utils::isImmediate(data, 1));

        return sys;
    }
} // namespace momiji::instr
