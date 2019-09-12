#include "swap.h"

#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void swap(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& /*labels*/,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& /*additionalData*/)
    {
        repr::Swap bits;

        bits.datareg = std::uint16_t(extractRegister(instr.operands[0]));

        opcode.val = std::uint16_t((bits.header << 3) | (bits.datareg));
    }
} // namespace momiji::enc
