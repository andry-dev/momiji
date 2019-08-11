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
} // namespace momiji
