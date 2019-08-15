#pragma once

#include <momiji/Parser.h>

#include <string_view>
#include <vector>

namespace momiji
{
    struct parser_metadata
    {
        bool result;
        std::string_view rest;
        std::string_view parsed_str;
        ParserError::ErrorType error { errors::UnknownError {} };
    };

    using math_parser_metadata =
        std::pair<parser_metadata,
                  std::unique_ptr<momiji::objects::MathASTNode>>;

    template <typename T>
    std::unique_ptr<momiji::objects::MathASTNode> make_node(T& t)
    {
        using momiji::objects::MathASTNode;

        return std::make_unique<MathASTNode>(std::move(t));
    }

    inline std::unique_ptr<momiji::objects::MathASTNode> make_node()
    {
        using momiji::objects::MathASTNode;

        return std::make_unique<MathASTNode>();
    }

} // namespace momiji
