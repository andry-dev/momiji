#include "Mappings.h"

namespace momiji::details
{
    details::ResultType parseMove(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        switch (instr.operands[0].operandType)
        {
        case OperandType::AddressRegister:
            instr.instructionType = InstructionType::MoveAddress;
            break;

        default:
            instr.instructionType = InstructionType::Move;
            break;
        }

        return res;
    }

    details::ResultType parseMoveQ(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = ImmediateInstructionParser(instr)(str);

        instr.instructionType = InstructionType::MoveQuick;

        return res;
    }

    details::ResultType parseAdd(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);
        switch (instr.operands[1].operandType)
        {
        case OperandType::AddressRegister:
            instr.instructionType = InstructionType::AddA;
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

    details::ResultType parseSub(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.instructionType = InstructionType::Sub;
        switch (instr.operands[1].operandType)
        {
        case OperandType::AddressRegister:
            instr.instructionType = InstructionType::SubA;
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

        return res;
    }

    details::ResultType parseMuls(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType = DataType::Word;
        instr.instructionType = InstructionType::SignedMul;

        return res;
    }

    details::ResultType parseMulu(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType = DataType::Word;
        instr.instructionType = InstructionType::UnsignedMul;

        return res;
    }

    details::ResultType parseDivs(std::string_view str, Instruction& instr, LabelInfo&)
    {
        auto res = CommonInstructionParser(instr)(str);

        instr.dataType = DataType::Word;
        instr.instructionType = InstructionType::SignedDiv;

        return res;
    }

    details::ResultType parseDivu(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseSwap(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseExg(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseOr(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseAnd(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseCmp(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseJmp(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseBra(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseBlt(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseBle(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseBgt(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseBge(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseBeq(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }

    details::ResultType parseBne(std::string_view str, Instruction& instr, LabelInfo&)
    {

    }
}
