#pragma once

#include <vector>
#include <string>
#include <variant>
#include <cstddef>

#include <momiji/Types.h>

namespace momiji
{
    // TODO(andry): The name is bad and not obvious.
    enum class ParserOperand : std::int8_t
    {
        DataRegister,
        AddressRegister,
        Address,
        AddressPre,
        AddressPost,
        AddressOffset,
        AddressIndex,
        Immediate,
        AbsoluteShort,
        AbsoluteLong,
        ProgramCounterOffset,
        ProgramCounterIndex
    };

    enum class ParserSection : std::int8_t
    {
        Code,
        Data
    };

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
            std::vector<momiji::ParserOperand> acceptedOps;
            momiji::ParserOperand inputOp;
            std::int8_t num { 0 };
        };

        struct DataTypeMismatch
        {
            std::vector<momiji::DataType> acceptedDataTypes;
            momiji::DataType inputDataType;
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

        struct UnexpectedSectionContent
        {
            ParserSection section;
        };
    }

    struct ParserError
    {
        std::int64_t line { 0 };
        std::int64_t column { 0 };

        using ErrorType =
            std::variant<momiji::errors::UnknownError,
                         momiji::errors::InvalidRegisterNumber,
                         momiji::errors::NoInstructionFound,
                         momiji::errors::NoLabelFound,
                         momiji::errors::OperandTypeMismatch,
                         momiji::errors::DataTypeMismatch,
                         momiji::errors::UnexpectedCharacter,
                         momiji::errors::MissingCharacter,
                         momiji::errors::UnknownOperand,
                         momiji::errors::UnexpectedSectionContent>;

        ErrorType errorType { momiji::errors::UnknownError {} };
        std::string codeStr;
    };

}

