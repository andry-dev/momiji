#include "swap.h"

#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void swap(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& /*additionalData*/)
    {
        repr::Swap bits;

        bits.datareg = instr.operands[0].value & 0b111;

        opcode.val = std::uint16_t((bits.header << 3) | (bits.datareg));
    }
} // namespace momiji::enc
