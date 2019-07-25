#include "bra.h"

#include "../Instructions/Representations.h"
#include "../Instructions/bcc.h"

namespace momiji::dec
{
    DecodedInstruction bcc(ExecutableMemoryView mem, std::uint64_t idx)
    {
        DecodedInstruction ret;

        repr::Bcc bits;

        const std::uint16_t val = mem.read16(idx);

        bits.condition    = (val & 0b00001111'00000000) >> 8;
        bits.displacement = (val & 0b00000000'11111111);

        ret.data.operandType[0] = static_cast<OperandType>(bits.condition);
        ret.data.operandType[1] = static_cast<OperandType>(bits.displacement);

        ret.exec = instr::bcc;

        auto cond = static_cast<momiji::BranchConditions>(bits.condition);

        switch (cond)
        {
        case BranchConditions::NotEqual:
            ret.string = "bne";
            break;

        case BranchConditions::Equal:
            ret.string = "beq";
            break;

        case BranchConditions::OverClear:
            break;

        case BranchConditions::OverSet:
            break;

        case BranchConditions::Plus:
            break;

        case BranchConditions::Minus:
            break;

        case BranchConditions::GreaterEq:
            ret.string = "bge";
            break;

        case BranchConditions::LessThan:
            ret.string = "blt";
            break;

        case BranchConditions::GreaterThan:
            ret.string = "bgt";
            break;

        case BranchConditions::LessEq:
            ret.string = "beq";
            break;
        }

        return ret;
    }
} // namespace momiji::dec
