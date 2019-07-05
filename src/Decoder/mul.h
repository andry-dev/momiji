#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction muls(ExecutableMemoryView mem, std::uint64_t idx);
    DecodedInstruction mulu(ExecutableMemoryView mem, std::uint64_t idx);
}
