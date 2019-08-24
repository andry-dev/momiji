#include "swap.h"

#include "../Instructions/Representations.h"
#include "../Instructions/swap.h"
#include "./Utils.h"

namespace momiji::dec
{
    DecodedInstruction swap(ConstExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        const std::uint16_t val = mem.read16(idx);

        repr::Swap bits;
        bits.datareg = (val & 0b00000000'00000111);

        ret.data.operandType[0] = OperandType::DataRegister;
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.datareg);

        ret.exec   = instr::swap;
        ret.string = "swap d" + std::to_string(bits.datareg);

        return ret;
    }
} // namespace momiji::dec
