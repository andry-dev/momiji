#include "swap.h"

#include "./Utils.h"

namespace momiji::instr
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"

    momiji::System swap(momiji::System& sys, const InstructionData& instr)
    {
        const auto datareg =
            std::int8_t(utils::to_val(instr.addressingMode[0]));

        std::int32_t& reg = asl::saccess(sys.cpu.dataRegisters, datareg).value;

        const auto higher = std::uint16_t((reg & 0xFFFF0000) >> 16);
        const auto lower  = std::uint16_t(reg & 0x0000FFFF);

        reg = std::int32_t((lower << 16) | higher);

        sys.cpu.programCounter.address += 2;

        return sys;
    }
#pragma clang diagnostic pop

} // namespace momiji::instr
