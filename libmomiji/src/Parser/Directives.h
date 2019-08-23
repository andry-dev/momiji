#pragma once

#include "./Common.h"
#include <momiji/Parser.h>
#include <string_view>

namespace momiji::details
{
    momiji::parser_metadata parseSection(std::string_view,
                                         momiji::ParsedInstruction&);
}
