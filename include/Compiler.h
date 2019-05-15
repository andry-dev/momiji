#pragma once

#include <vector>
#include <Parser.h>

namespace momiji
{
    struct CompilerSettings
    {
        std::int32_t programStart = 0;
    };

    std::vector<std::uint16_t>
    compile(const std::vector<momiji::Instruction>& instructions);
}
