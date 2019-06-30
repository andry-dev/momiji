#pragma once

#include <Parser.h>
#include <vector>

namespace momiji
{
    struct CompilerSettings
    {
        std::int32_t programStart = 0;
    };

    std::vector<std::uint16_t>
    compile(const std::vector<momiji::Instruction>& instructions);
} // namespace momiji
