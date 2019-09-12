#include "rts.h"

#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void rts(const momiji::ParsedInstruction& /*instr*/,
             const momiji::LabelInfo& /*labels*/,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& /*additionalData*/)
    {
        repr::Rts bits;
        opcode.val = bits.header;
    }
} // namespace momiji::enc
