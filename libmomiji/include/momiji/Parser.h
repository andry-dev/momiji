#pragma once

#include "Types.h"

#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include <gsl/span>

#include "expected.hpp"

namespace momiji
{
    struct System;
    struct Breakpoint
    {
        std::int64_t source_line { 0 };
    };

    struct ParserSettings
    {
        gsl::span<Breakpoint> breakpoints;
    };

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
    } // namespace errors

    namespace warnings
    {

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

    namespace objects
    {
        struct Label
        {
            std::uint32_t hash;
        };

        struct Number
        {
            std::int32_t number;
        };

        struct MathASTNode;

        struct MathOperator
        {
            MathOperator()  = default;
            ~MathOperator() = default;

            MathOperator(const MathOperator& oth) = delete;
            MathOperator& operator=(const MathOperator& oth) = delete;

            MathOperator(MathOperator&&) = default;
            MathOperator& operator=(MathOperator&&) = default;

            enum class Type
            {
                Add,
                Sub,
                Mul,
                Div,
            };

            Type type;

            std::unique_ptr<MathASTNode> left;
            std::unique_ptr<MathASTNode> right;
        };

        struct MathASTNode
        {
            MathASTNode() = default;

            MathASTNode(Label&& label)
                : value(std::move(label))
            {
            }

            MathASTNode(Number&& num)
                : value(std::move(num))
            {
            }

            MathASTNode(MathOperator&& op)
                : value(std::move(op))
            {
            }

            std::variant<Label, Number, MathOperator> value;
        };
    } // namespace objects

    namespace operands
    {
        // While the value can be encoded in 4 bits, error checking would be
        // harder.
        // Besides, because of the std::unique_ptr<> we can afford to waste some
        // bytes anyway.

        struct DataRegister
        {
            std::int32_t reg;
        };

        struct AddressRegister
        {
            std::int32_t reg;
        };

        struct Address
        {
            // std::unique_ptr<objects::MathASTNode> offset;
            std::int32_t reg;
        };

        struct AddressPre
        {
            std::int32_t reg;
        };

        struct AddressPost
        {
            std::int32_t reg;
        };

        struct AddressOffset
        {
            std::unique_ptr<objects::MathASTNode> offset;
            std::int32_t reg;
        };

        struct AddressIndex
        {
            std::unique_ptr<objects::MathASTNode> offset;
            std::int32_t reg;
            std::int32_t othreg;
        };

        struct Immediate
        {
            std::unique_ptr<objects::MathASTNode> value;
        };

        struct AbsoluteShort
        {
            std::unique_ptr<objects::MathASTNode> value;
        };

        struct AbsoluteLong
        {
            std::unique_ptr<objects::MathASTNode> value;
        };

        // TODO(andry): Implement these

        struct ProgramCounterOffset
        {
        };

        struct ProgramCounterIndex
        {
        };
    } // namespace operands

    using Operand = std::variant<operands::DataRegister,
                                 operands::AddressRegister,
                                 operands::Address,
                                 operands::AddressPre,
                                 operands::AddressPost,
                                 operands::AddressOffset,
                                 operands::AddressIndex,
                                 operands::Immediate,
                                 operands::ProgramCounterOffset,
                                 operands::ProgramCounterIndex,
                                 operands::AbsoluteShort,
                                 operands::AbsoluteLong>;

    struct ParsedInstruction
    {
        ParsedInstruction()
            : operands(2)
        {
        }

        std::vector<Operand> operands;
        DataType dataType;
        InstructionType instructionType;
        BranchConditions branchCondition;
        std::int32_t programCounter;
        std::int32_t sourceLine;
    };

    struct Label
    {
        Label(std::uint32_t nameHash, std::int64_t idx, std::string_view string)
            : string(string)
            , idx(idx)
            , nameHash(nameHash)
        {
        }

        Label()  = default;
        ~Label() = default;

        Label(const Label&) = default;
        Label& operator=(const Label&) = default;

        Label(Label&&) = default;
        Label& operator=(Label&&) = default;

        std::string string {};
        std::int64_t idx { 0 };
        std::uint32_t nameHash { 0 };
    };

    using LabelInfo = std::vector<momiji::Label>;

    struct ParsingInfo
    {
        std::vector<momiji::ParsedInstruction> instructions;
        momiji::LabelInfo labels;
    };

    using ParsingResult =
        nonstd::expected<momiji::ParsingInfo, momiji::ParserError>;

    momiji::ParsingResult parse(const std::string& str);
    momiji::ParsingResult parse(const std::string& str,
                                ParserSettings settings);

    std::int32_t resolveAST(const objects::MathASTNode& node,
                            const momiji::LabelInfo& labels);

    std::int32_t extractRegister(const Operand& operand);
    std::int32_t extractASTValue(const Operand& operand,
                                 const LabelInfo& labels);

    momiji::ParserOperand convertOperand(const momiji::Operand& operand);

    template <typename OperandType, typename Fun>
    inline bool mapOperand(Operand& op, Fun&& fn)
    {
        if (std::holds_alternative<OperandType>(op))
        {
            fn(op);
            return true;
        }

        return false;
    }

    template <typename OperandType>
    inline bool matchOperand(const Operand& op)
    {
        return std::holds_alternative<OperandType>(op);
    }

    namespace details
    {
        template <typename T, typename = void>
        struct has_reg_var : std::false_type
        {
        };

        template <typename T>
        struct has_reg_var<T, std::void_t<decltype(std::declval<T>().reg)>>
            : std::true_type
        {
        };

        template <typename T, typename = void>
        struct has_value_var : std::false_type
        {
        };

        template <typename T>
        struct has_value_var<T, std::void_t<decltype(std::declval<T>().value)>>
            : std::true_type
        {
        };

        struct RegVisitor
        {
            template <typename T>
            void operator()(const T& reg)
            {
                if constexpr (details::has_reg_var<T>::value)
                {
                    val = reg.reg;
                }
            }

            std::int32_t val { 0 };
        };

        struct ValueVisitor
        {
            ValueVisitor(const LabelInfo& labels)
                : labels(labels)
            {
            }

            template <typename T>
            void operator()(const T& value)
            {
                if constexpr (details::has_value_var<T>::value)
                {
                    val = momiji::resolveAST(*value.value, labels);
                }
            }

            std::int32_t val { 0 };
            const LabelInfo& labels;
        };
    } // namespace details

} // namespace momiji
