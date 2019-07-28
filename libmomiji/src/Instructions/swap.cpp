#include "swap.h"

#include "./Utils.h"

namespace momiji::instr
{
    momiji::System swap(momiji::System& sys, const InstructionData& instr)
    {
        const std::int8_t datareg = utils::to_val(instr.addressingMode[0]);

        std::int32_t& reg = sys.cpu.dataRegisters[datareg].value;

        std::int16_t higher = (reg & 0xFFFF0000) >> 16;
        std::int16_t lower  = (reg & 0x0000FFFF);

        reg = (lower << 16) | higher;

        sys.cpu.programCounter.address += 2;

        return sys;
    }
} // namespace momiji::instr
