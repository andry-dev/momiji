#pragma once

#include <System.h>
#include <Decoder.h>

namespace momiji::dec
{
    DecodedInstruction add(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction adda(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction addi(ExecutableMemoryView mem, std::uint64_t idx);
}
