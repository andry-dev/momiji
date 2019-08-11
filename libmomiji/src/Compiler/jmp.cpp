#include "jmp.h"

#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void jmp(const momiji::ParsedInstruction& instr,
             const momiji::LabelInfo& labels,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Jmp bits;

        const auto& op = instr.operands[0];

        bits.regtype = getCorrectOpType(op);
        bits.regmode = getCorrectOpMode(op);

        if (matchOperand<operands::AbsoluteShort>(op) ||
            matchOperand<operands::AbsoluteLong>(op))
        {
            additionalData[0].val = extractASTValue(op, labels);
            additionalData[0].cnt = 4;
        }

        opcode.val = std::uint16_t((bits.header << 6) | (bits.regtype << 3) |
                                   (bits.regmode));
    }

    void jsr(const momiji::ParsedInstruction& instr,
             const momiji::LabelInfo& labels,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& /*additionalData*/)
    {
        repr::Jsr bits;

        bits.regtype = getCorrectOpType(instr.operands[0]);
        bits.regmode = getCorrectOpMode(instr.operands[0]);

        opcode.val = std::uint16_t((bits.header << 6) | (bits.regtype << 3) |
                                   (bits.regmode));
    }
} // namespace momiji::enc
