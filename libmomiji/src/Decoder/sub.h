#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::dec
{
    DecodedInstruction sub(ExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction suba(ExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction subi(ExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
