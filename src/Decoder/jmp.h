#pragma once

#include <System.h>
#include <Decoder.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction jmp(ExecutableMemoryView mem, std::uint64_t idx);
}
