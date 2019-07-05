#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::dec
{
    DecodedInstruction sub(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction suba(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction subi(ExecutableMemoryView mem, std::uint64_t idx);
}
