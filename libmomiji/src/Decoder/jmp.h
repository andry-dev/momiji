#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction jmp(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction jsr(ExecutableMemoryView mem, std::uint64_t idx);
} // namespace momiji::dec
