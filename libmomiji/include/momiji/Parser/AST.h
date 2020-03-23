#pragma once

#include <memory>
#include <cstddef>
#include <variant>

#include <momiji/Parser/Common.h>

namespace momiji::objects
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
            : value(label)
        {
        }

        MathASTNode(Number&& num)
            : value(num)
        {
        }

        MathASTNode(MathOperator&& op)
            : value(std::move(op))
        {
        }

        std::variant<Label, Number, MathOperator> value;
    };

}

namespace momiji
{
    // TODO(andry): The name is bad
    std::int32_t resolveAST(const objects::MathASTNode& node,
                            const momiji::LabelInfo& labels);
}
