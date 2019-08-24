#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction move(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction moveq(ConstExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
