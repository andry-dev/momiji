#include "Instructions.h"

namespace momiji::details
{
    static bool isImmediate(const Operand& op)
    {
        return op.operandType == OperandType::Immediate &&
               op.specialAddressingMode == SpecialAddressingMode::Immediate;
    }

    static bool sanitizeRegisters(const Operand& op, parser_metadata& metadata)
    {
        if ((op.operandType != OperandType::Immediate) &&
            (op.value < 0 || op.value > 7))
        {
            metadata.result = false;
            metadata.error.errorType = ParserError::ErrorType::WrongRegisterNumber;

            return false;
        }

        return true;
    }

    static bool sanitizeImmediate(const Operand& op, parser_metadata& metadata)
    {
        if (isImmediate(op))
        {
            metadata.result = false;
            metadata.error.errorType = ParserError::ErrorType::WrongOperandType;

            return false;
        }

        return true;
    }

    template <typename T, typename... U>
    static bool not_eq_all(T& x, U&&... y)
    {
        return ((x != y) && ...);
    }

    parser_metadata parseMove(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        for (const auto& op : instr.operands)
        {
            switch (op.operandType)
            {
            case OperandType::AddressRegister:
            case OperandType::DataRegister:
                sanitizeRegisters(op, res);
                break;
            }
        }

        sanitizeImmediate(instr.operands[1], res);

        instr.instructionType = InstructionType::Move;

        return res;
    }

    parser_metadata parseMoveQ(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = ImmediateInstructionParser(instr)(str);

        sanitizeRegisters(instr.operands[1], res);

        instr.instructionType = InstructionType::MoveQuick;


        return res;
    }

    parser_metadata parseAdd(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);
        switch (instr.operands[1].operandType)
        {
        case OperandType::AddressRegister:
            instr.instructionType = InstructionType::AddA;
            break;

        case OperandType::Immediate:
            sanitizeImmediate(instr.operands[1], res);
            break;

        default:
            instr.instructionType = InstructionType::Add;
            break;
        }

        switch (instr.operands[0].operandType)
        {
        case OperandType::Immediate:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::Immediate:
                instr.instructionType = InstructionType::AddI;
                break;
            }
            break;
        }

        return res;
    }

    parser_metadata parseSub(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.instructionType = InstructionType::Sub;
        switch (instr.operands[1].operandType)
        {
        case OperandType::AddressRegister:
            instr.instructionType = InstructionType::SubA;
            break;

        case OperandType::Immediate:
            sanitizeImmediate(instr.operands[1], res);
            break;

        default:
            instr.instructionType = InstructionType::Sub;
            break;
        }

        switch (instr.operands[0].operandType)
        {
        case OperandType::Immediate:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::Immediate:
                instr.instructionType = InstructionType::SubI;
                break;
            }
            break;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        return res;
    }

    parser_metadata parseMuls(std::string_view str, Instruction& instr,
                              LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        instr.dataType = DataType::Word;
        instr.instructionType = InstructionType::SignedMul;

        return res;
    }

    parser_metadata parseMulu(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType = DataType::Word;
        instr.instructionType = InstructionType::UnsignedMul;

        return res;
    }

    parser_metadata parseDivs(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType = DataType::Word;
        instr.instructionType = InstructionType::SignedDiv;

        return res;
    }

    parser_metadata parseDivu(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType = DataType::Word;
        instr.instructionType = InstructionType::UnsignedDiv;

        return res;
    }

    parser_metadata parseSwap(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = OneRegisterInstructionParser(instr)(str);

        instr.dataType = DataType::Long;
        instr.instructionType = InstructionType::Swap;

        return res;
    }

    parser_metadata parseExg(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType = DataType::Word;
        instr.instructionType = InstructionType::Exchange;

        return res;
    }

    parser_metadata parseOr(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        switch (instr.operands[0].operandType)
        {
        case OperandType::Immediate:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::Immediate:
                instr.instructionType = InstructionType::OrI;
                break;
            }
            break;

        default:
            instr.instructionType = InstructionType::Or;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        return res;
    }

    parser_metadata parseAnd(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        switch (instr.operands[0].operandType)
        {
        case OperandType::Immediate:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::Immediate:
                instr.instructionType = InstructionType::AndI;
                break;
            }
            break;

        default:
            instr.instructionType = InstructionType::And;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        return res;
    }

    parser_metadata parseCmp(std::string_view str, Instruction& instr, LabelInfo&)
    {

        auto res = CommonInstructionParser(instr)(str);

        switch (instr.operands[1].operandType)
        {
        case OperandType::AddressRegister:
            instr.instructionType = InstructionType::CompareA;
            break;

        default:
            instr.instructionType = InstructionType::Compare;
        }

        switch (instr.operands[0].operandType)
        {
        case OperandType::Immediate:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::Immediate:
                instr.instructionType = InstructionType::CompareI;
                break;
            }
            break;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        return res;
    }

    parser_metadata parseJmp(std::string_view str, Instruction& instr,
                             LabelInfo& labels)
    {
        auto res = SeqNext(Whitespace(),
                           AnyOf(MemoryAddress(instr, 0),
                                 AsAddress(instr, 0)))(str);

        instr.instructionType = InstructionType::Jmp;

        switch (instr.operands[0].operandType)
        {
        case OperandType::Address:
            sanitizeRegisters(instr.operands[0], res);
            break;

        case OperandType::AbsoluteLong:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::AbsoluteLong:
            case SpecialAddressingMode::AbsoluteShort:
                break;

            default:
                res.result = false;
                res.error.errorType = ParserError::ErrorType::WrongOperandType;
            }
            break;

        default:
            res.result = false;
            res.error.errorType = ParserError::ErrorType::WrongOperandType;
        }

        return res;
    }

    parser_metadata parseBra(std::string_view str, Instruction& instr,
                             LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::Branch;

        return res;
    }

    parser_metadata parseBlt(std::string_view str, Instruction& instr,
                                 LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::LessThan;

        return res;
    }

    parser_metadata parseBle(std::string_view str, Instruction& instr,
                                 LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::LessEq;

        return res;
    }

    parser_metadata parseBgt(std::string_view str, Instruction& instr,
                                 LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::GreaterThan;

        return res;
    }

    parser_metadata parseBge(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::GreaterEq;

        return res;
    }

    parser_metadata parseBeq(std::string_view str, Instruction& instr,
                                 LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::Equal;

        return res;
    }

    parser_metadata parseBne(std::string_view str, Instruction& instr,
                             LabelInfo& labels)
    {

        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::NotEqual;

        return res;
    }
} // namespace momiji::details
