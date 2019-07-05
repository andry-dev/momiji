#pragma once

#include <Parser.h>
#include <Memory.h>

namespace momiji
{
    struct CompilerSettings
    {
        std::int32_t programStart = 0;
    };

    ExecutableMemory
    compile(const std::vector<momiji::Instruction>& instructions);
} // namespace momiji
