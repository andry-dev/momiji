#pragma once

#include "Types.h"
#include <array>
#include <string>
#include <vector>
#include <string_view>

#include "expected.hpp"

#include "System.h"

namespace momiji
{
    struct Instruction;

    using instr_fn_t = momiji::System(*)(momiji::System,
                                        const Instruction&);

    struct Operand
    {
        std::int32_t value;
        OperandType operandType;
        RegisterType registerType;
    };

    struct Instruction
    {
        std::array<Operand, 3> operands;

        InstructionType instructionType;
        DataType dataType;
        std::int8_t numOperands;

        instr_fn_t executefn;
    };


    struct ParserError
    {
        int line{0};
        int column{0};

        enum class ErrorType
        {
            NoInstructionFound,
            NoLabelFound,
            WrongInstruction,
            WrongOperandType,
            UnexpectedCharacter,
            Comment,
        } errorType;
    };

    using ParsingResult = nonstd::expected<std::vector<momiji::Instruction>, ParserError>;


    nonstd::expected<momiji::Label, ParserError> readLabel(const std::string& str);
    nonstd::expected<momiji::Instruction, ParserError> readInstruction(const std::string& str);

    momiji::ParsingResult parse(const std::string& str);


}
