#pragma once

#include "Types.h"
#include <array>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include <gsl/span>

#include "expected.hpp"

namespace momiji
{
    namespace errors
    {
        struct NoLabelFound
        {
            std::string label;
        };

        struct NoInstructionFound
        {
            std::vector<std::string> alternatives;
            std::string inputString;
        };

        struct OperandTypeMismatch
        {
            std::vector<OperandType> acceptedOps;
            OperandType inputOp;
            std::int8_t num { 0 };
        };

        struct InvalidRegisterNumber
        {
            std::int32_t input;
        };

        struct UnexpectedCharacter
        {
            char character;
        };

        struct MissingCharacter
        {
            char character;
        };

        struct UnknownOperand
        {
        };

        struct UnknownError
        {
        };
    } // namespace errors

    namespace warnings
    {

    }

    struct Instruction;
    struct System;

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

    struct InstructionMetadata
    {
      private:
        std::string_view codeStr;

        friend struct Parser;

      public:
        std::int64_t sourceLine;
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
        InstructionMetadata metadata;
        std::int64_t programCounter { 0 };
    };

    struct ParserError
    {
        std::int64_t line { 0 };
        std::int64_t column { 0 };

        using ErrorType = std::variant<errors::UnknownError,
                                       errors::InvalidRegisterNumber,
                                       errors::NoInstructionFound,
                                       errors::NoLabelFound,
                                       errors::OperandTypeMismatch,
                                       errors::UnexpectedCharacter,
                                       errors::MissingCharacter,
                                       errors::UnknownOperand>;

        ErrorType errorType { errors::UnknownError {} };
        std::string codeStr;
    };

    using ParsingResult =
        nonstd::expected<std::vector<momiji::Instruction>, ParserError>;

    momiji::ParsingResult parse(const std::string& str);
    momiji::ParsingResult parse(const std::string& str,
                                ParserSettings settings);

} // namespace momiji
