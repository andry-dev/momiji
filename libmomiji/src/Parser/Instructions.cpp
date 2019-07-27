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
            ((op.value & 0b111) < 0 || (op.value & 0b111) > 7))
        {
            metadata.result = false;
            metadata.error.errorType =
                ParserError::ErrorType::WrongRegisterNumber;

            return false;
        }

        return true;
    }

    static bool sanitizeImmediate(const Operand& op, parser_metadata& metadata)
    {
        if (isImmediate(op))
        {
            metadata.result          = false;
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

    parser_metadata
    parseMove(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        instr.instructionType = InstructionType::Move;

        return res;
    }

    parser_metadata
    parseMoveQ(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = ImmediateInstructionParser(instr)(str);

        sanitizeRegisters(instr.operands[1], res);

        instr.instructionType = InstructionType::MoveQuick;

        return res;
    }

    parser_metadata
    parseAdd(std::string_view str, Instruction& instr, LabelInfo&)
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

            default:
                break;
            }
            break;

        default:
            break;
        }

        return res;
    }

    parser_metadata
    parseSub(std::string_view str, Instruction& instr, LabelInfo&)
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

    parser_metadata
    parseMuls(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::SignedMul;

        return res;
    }

    parser_metadata
    parseMulu(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::UnsignedMul;

        return res;
    }

    parser_metadata
    parseDivs(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::SignedDiv;

        return res;
    }

    parser_metadata
    parseDivu(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::UnsignedDiv;

        return res;
    }

    parser_metadata
    parseSwap(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = OneRegisterInstructionParser(instr)(str);

        instr.dataType        = DataType::Long;
        instr.instructionType = InstructionType::Swap;

        return res;
    }

    parser_metadata
    parseExg(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType        = DataType::Word;
        instr.instructionType = InstructionType::Exchange;

        return res;
    }

    parser_metadata
    parseOr(std::string_view str, Instruction& instr, LabelInfo&)
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

    parser_metadata
    parseAnd(std::string_view str, Instruction& instr, LabelInfo&)
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

    parser_metadata
    parseNot(std::string_view str, Instruction& instr, LabelInfo&)
    {
        return { false, str, "", {} };
    }

    parser_metadata
    parseXor(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        switch (instr.operands[0].operandType)
        {
        case OperandType::Immediate:
            switch (instr.operands[0].specialAddressingMode)
            {
            case SpecialAddressingMode::Immediate:
                instr.instructionType = InstructionType::XorI;
                break;
            }
            break;

        default:
            instr.instructionType = InstructionType::Xor;
        }

        for (const auto& op : instr.operands)
        {
            sanitizeRegisters(op, res);
        }

        sanitizeImmediate(instr.operands[1], res);

        if (instr.operands[0].operandType != OperandType::DataRegister)
        {
            res.result          = false;
            res.error.errorType = ParserError::ErrorType::WrongOperandType;
        }

        return res;
    }

    parser_metadata
    parseCmp(std::string_view str, Instruction& instr, LabelInfo&)
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

    parser_metadata
    parseTst(std::string_view str, Instruction& instr, LabelInfo&)
    {
        instr.instructionType = InstructionType::Tst;
        return OneRegisterInstructionParser(instr)(str);
    }

    parser_metadata
    parseJmp(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res =
            SeqNext(Whitespace(),
                    AnyOf(MemoryAddress(instr, 0), AsAddress(instr, 0)))(str);

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
                res.result          = false;
                res.error.errorType = ParserError::ErrorType::WrongOperandType;
            }
            break;

        default:
            res.result          = false;
            res.error.errorType = ParserError::ErrorType::WrongOperandType;
        }

        return res;
    }

    parser_metadata
    parseJsr(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res = parseJmp(str, instr, labels);

        instr.instructionType = InstructionType::JmpSubroutine;

        return res;
    }

    parser_metadata
    parseBra(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::Branch;

        return res;
    }

    parser_metadata
    parseBsr(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res = parseBra(str, instr, labels);

        instr.instructionType = InstructionType::BranchSubroutine;

        return res;
    }

    parser_metadata
    parseRts(std::string_view str, Instruction& instr, LabelInfo&)
    {
        instr.instructionType         = InstructionType::ReturnSubroutine;
        instr.operands[0].operandType = OperandType::DataRegister;
        instr.operands[1].operandType = OperandType::DataRegister;

        return { true, str, "", {} };
    }

    parser_metadata
    parseBlt(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::LessThan;

        return res;
    }

    parser_metadata
    parseBle(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::LessEq;

        return res;
    }

    parser_metadata
    parseBgt(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::GreaterThan;

        return res;
    }

    parser_metadata
    parseBge(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::GreaterEq;

        return res;
    }

    parser_metadata
    parseBeq(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::Equal;

        return res;
    }

    parser_metadata
    parseBne(std::string_view str, Instruction& instr, LabelInfo& labels)
    {

        auto res = BranchInstructionParser(instr, labels)(str);

        instr.instructionType = InstructionType::BranchCondition;
        instr.branchCondition = BranchConditions::NotEqual;

        return res;
    }

    parser_metadata
    parseAsl(std::string_view str, Instruction& instr, LabelInfo&)
    {
        instr.instructionType = InstructionType::ArithmeticShiftLeft;

        return ShiftInstructionParser(instr)(str);
    }

    parser_metadata
    parseAsr(std::string_view str, Instruction& instr, LabelInfo&)
    {
        instr.instructionType = InstructionType::ArithmeticShiftRight;

        return ShiftInstructionParser(instr)(str);
    }

    parser_metadata
    parseLsl(std::string_view str, Instruction& instr, LabelInfo&)
    {
        instr.instructionType = InstructionType::LogicalShiftLeft;

        return ShiftInstructionParser(instr)(str);
    }

    parser_metadata
    parseLsr(std::string_view str, Instruction& instr, LabelInfo&)
    {
        instr.instructionType = InstructionType::LogicalShiftRight;

        return ShiftInstructionParser(instr)(str);
    }

    parser_metadata
    parseDeclare(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        parser_metadata res = (ParseDataType(instr))(str);

        instr.instructionType = InstructionType::Declare;

        auto singleOpParser = [](Instruction& instr, int opNum) {
            return [&instr, opNum](std::string_view str) -> parser_metadata {
                while (opNum >= instr.operands.size())
                {
                    instr.operands.emplace_back();
                }

                auto res = MemoryAddress(instr, opNum);

                return res(str);
            };
        };

        for (int i = 0; true; ++i)
        {
            auto tryNum = SeqNext(AlwaysTrue(Whitespace()),
                                  singleOpParser(instr, i))(res.rest);

            if (tryNum.result)
            {
                res = tryNum;
                auto tryComma =
                    SeqNext(AlwaysTrue(Whitespace()), Char(','))(res.rest);
                if (tryComma.result)
                {
                    res = tryComma;
                }
                else
                {
                    return res;
                }
            }
            else
            {
                return res;
            }
        }

        return res;
    }

    parser_metadata
    parseHcf(std::string_view str, Instruction& instr, LabelInfo& labels)
    {
        instr.instructionType = InstructionType::HaltCatchFire;

        instr.operands[0].operandType = OperandType::Immediate;
        instr.operands[0].specialAddressingMode =
            SpecialAddressingMode::Immediate;

        instr.operands[1].operandType = OperandType::DataRegister;

        instr.dataType = DataType::Word;

        return { true, str, "", {} };
    }

} // namespace momiji::details
