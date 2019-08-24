#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction muls(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction mulu(ConstExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
