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

    struct ParserError
    {
        std::int64_t line { 0 };
        std::int64_t column { 0 };

        using ErrorType = std::variant<momiji::errors::UnknownError,
                                       momiji::errors::InvalidRegisterNumber,
                                       momiji::errors::NoInstructionFound,
                                       momiji::errors::NoLabelFound,
                                       momiji::errors::OperandTypeMismatch,
                                       momiji::errors::UnexpectedCharacter,
                                       momiji::errors::MissingCharacter,
                                       momiji::errors::UnknownOperand>;

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
            MathASTNode()  = default;
            ~MathASTNode() = default;

            MathASTNode(const MathASTNode&) = delete;
            MathASTNode& operator=(const MathASTNode&) = delete;

            MathASTNode(MathASTNode&&) = default;
            MathASTNode& operator=(MathASTNode&&) = default;

            std::variant<Label, Number, MathOperator> value;
        };
    } // namespace objects

    namespace operands
    {
        struct DataRegister
        {
            std::int8_t reg : 4;
        };

        struct AddressRegister
        {
            std::int8_t reg : 4;
        };

        struct Address
        {
            // std::unique_ptr<objects::MathASTNode> offset;
            std::int8_t reg : 4;
        };

        struct AddressPre
        {
            std::int8_t reg : 4;
        };

        struct AddressPost
        {
            std::int8_t reg : 4;
        };

        struct AddressOffset
        {
            std::unique_ptr<objects::MathASTNode> offset;
            std::int8_t reg : 4;
        };

        struct AddressIndex
        {
            std::unique_ptr<objects::MathASTNode> offset;
            std::int8_t reg : 4;
            std::int8_t othreg : 4;
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

    static_assert(sizeof(momiji::ParsedInstruction) == 40);

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

    static_assert(sizeof(momiji::Label) == 48);

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

    std::int8_t extractRegister(const Operand& operand);
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

            std::int8_t val { 0 };
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
