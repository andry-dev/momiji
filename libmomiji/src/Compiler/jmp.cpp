#include "jmp.h"

#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void jmp(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Jmp bits;

        const auto& op = instr.operands[0];

        bits.regtype = utils::to_val(op.operandType);
        bits.regmode = getCorrectOpMode(instr, 0);

        if (op.operandType == OperandType::Immediate &&
            (op.specialAddressingMode == SpecialAddressingMode::AbsoluteLong ||
             op.specialAddressingMode == SpecialAddressingMode::AbsoluteShort))
        {
            additionalData[0].val = op.value;
            additionalData[0].cnt = 4;
        }

        opcode.val = (bits.header << 6) | (bits.regtype << 3) | (bits.regmode);
    }

    void jsr(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Jsr bits;

        bits.regtype = utils::to_val(instr.operands[0].operandType);
        bits.regmode = getCorrectOpMode(instr, 0);

        opcode.val = (bits.header << 6) | (bits.regtype << 3) | (bits.regmode);
    }
} // namespace momiji::enc
