#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction or_instr(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction ori(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction ora(ExecutableMemoryView mem, std::uint64_t idx);
} // namespace momiji::dec
