#pragma once

#include "Types.h"
#include <array>
#include <string>
#include <vector>
#include <string_view>

#include "expected.hpp"

namespace momiji
{
    struct Instruction;
    struct System;
    struct Label;

    using instr_fn_t = momiji::System(*)(momiji::System,
                                        const Instruction&);

    struct Operand
    {
        union
        {
            std::int32_t value;
            std::int16_t half[2];
            std::int8_t single[4];
        };

        OperandType operandType;
        SpecialAddressingMode specialAddressingMode;
    };

    struct Instruction
    {
        std::array<Operand, 3> operands;

        InstructionType instructionType;
        BranchConditions branchCondition;
        DataType dataType;
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
        } errorType;
    };

    using ParsingResult = nonstd::expected<
        std::vector<momiji::Instruction>,
        ParserError
    >;

    momiji::ParsingResult parse(const std::string& str);


}
