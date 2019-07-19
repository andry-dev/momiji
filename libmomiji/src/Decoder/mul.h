#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction muls(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction mulu(ExecutableMemoryView mem, std::uint64_t idx);
} // namespace momiji::dec
