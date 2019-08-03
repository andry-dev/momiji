#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::dec
{
    DecodedInstruction add(ExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction adda(ExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction addi(ExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
