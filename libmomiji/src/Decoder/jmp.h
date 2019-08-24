#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction jmp(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction jsr(ConstExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
