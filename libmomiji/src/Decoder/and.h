#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji
{
    namespace dec
    {
        DecodedInstruction and_instr(ConstExecutableMemoryView mem,
                                     std::int64_t idx);
        DecodedInstruction andi(ConstExecutableMemoryView mem,
                                std::int64_t idx);
    } // namespace dec
} // namespace momiji
