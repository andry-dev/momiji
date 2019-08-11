#pragma once

#include <momiji/Memory.h>
#include <momiji/Parser.h>

namespace momiji
{
    struct CompilerSettings
    {
        std::int32_t programStart = 0;
    };

    ExecutableMemory compile(const momiji::ParsingInfo& instructions);
} // namespace momiji
