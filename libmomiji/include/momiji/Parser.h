#pragma once

#include "Types.h"
#include <array>
#include <string>
#include <string_view>
#include <vector>

#include <gsl/span>

#include "expected.hpp"

namespace momiji
{
    struct Instruction;
    struct System;
    struct Label;

    using instr_fn_t = momiji::System (*)(momiji::System, const Instruction&);

    struct Breakpoint
    {
        std::int64_t source_line { 0 };
    };

    struct ParserSettings
    {
        gsl::span<Breakpoint> breakpoints;
    };

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
        bool labelResolved { true };
    };

    struct Instruction
    {
        Instruction()
            : operands(2)
        {
        }

        std::vector<Operand> operands;

        InstructionType instructionType;
        BranchConditions branchCondition;
        DataType dataType;
        std::int64_t programCounter { 0 };
    };

    struct ParserError
    {
        std::int64_t line { 0 };
        std::int64_t column { 0 };

        enum class ErrorType
        {
            NoInstructionFound,
            NoLabelFound,
            WrongInstruction,
            WrongOperandType,
            WrongRegisterNumber,
            UnexpectedCharacter,
        } errorType;

        std::string codeStr;
    };

    using ParsingResult =
        nonstd::expected<std::vector<momiji::Instruction>, ParserError>;

    momiji::ParsingResult parse(const std::string& str);
    momiji::ParsingResult parse(const std::string& str,
                                ParserSettings settings);

} // namespace momiji
