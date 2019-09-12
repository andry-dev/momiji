#include "bcc.h"

#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void bcc(const momiji::ParsedInstruction& instr,
             const momiji::LabelInfo& labels,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Bcc bits;

        bits.condition = utils::to_val(instr.branchCondition) & 0b1111;

        const auto val          = extractASTValue(instr.operands[0], labels);
        const auto displacement = val - instr.programCounter;

#ifdef LIBMOMIJI_CORRECT_BRA_IMPL
        std::uint16_t tmp = instr.operands[0].value;
        if (tmp > 255)
        {
            bits.displacement = 0;

            additionalData[0].val = displacement;
            additionalData[0].cnt = 2;
        }
        else
        {
            bits.displacement = displacement & 0x000000FF;
        }
#else
        bits.displacement = 0;

        additionalData[0].val = std::uint32_t(displacement);
        additionalData[0].cnt = 2;
#endif

        opcode.val = std::uint16_t((bits.header << 12) | (bits.condition << 8) |
                                   (bits.displacement));
    }
} // namespace momiji::enc
