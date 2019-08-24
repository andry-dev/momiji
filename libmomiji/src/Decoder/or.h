#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji::dec
{
    DecodedInstruction or_instr(ConstExecutableMemoryView mem,
                                std::int64_t idx);
    DecodedInstruction ori(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction ora(ConstExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
