#include "swap.h"

#include "./Utils.h"

#include <asl/detect_features>

namespace momiji::instr
{
#ifdef ASL_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif

    momiji::System swap(momiji::System& sys, const InstructionData& instr)
    {
        const auto datareg =
            std::int8_t(utils::to_val(instr.addressingMode[0]));

        auto& reg = asl::saccess(sys.cpu.dataRegisters, datareg);

        const auto higher = (reg & 0xFFFF0000) >> 16;
        const auto lower  = (reg & 0x0000FFFF);

        reg = (lower << 16) | higher;

        sys.cpu.programCounter += 2;

        return sys;
    }
#ifdef ASL_CLANG
#pragma clang diagnostic pop
#endif

} // namespace momiji::instr
