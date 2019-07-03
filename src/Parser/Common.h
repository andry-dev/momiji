#pragma once

#include <string_view>
#include <Parser.h>
#include <vector>

namespace momiji
{
    struct parser_metadata
    {
        bool result;
        std::string_view rest;
        std::string_view parsed_str;
        ParserError error;
    };

    struct LabelInfo
    {
        std::vector<momiji::Label> labels;
    };
}
