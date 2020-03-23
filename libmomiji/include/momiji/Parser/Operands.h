#pragma once

#include <Parser/Errors.h>
#include <cstddef>
#include <memory>
#include <variant>

#include <momiji/Parser/AST.h>
#include <momiji/Parser/Common.h>

namespace momiji::operands
{
    // While the value can be encoded in 4 bits, error checking would be
    // harder.
    // Besides, because of the std::unique_ptr<> we can afford to waste some
    // bytes anyway.

    struct DataRegister
    {
        std::int32_t reg;

        static momiji::ParserOperand error()
        {
            return momiji::ParserOperand::DataRegister;
        }
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
}

namespace momiji
{
    // TODO(andry): The name is bad
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

    std::int32_t extractRegister(const Operand& operand);
    std::int32_t extractASTValue(const Operand& operand,
                                 const LabelInfo& labels);
}
