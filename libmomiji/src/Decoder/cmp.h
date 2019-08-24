#pragma once

#include <Decoder.h>
#include <System.h>
#include <gsl/span>

namespace momiji
{
    namespace dec
    {
        DecodedInstruction cmp(ConstExecutableMemoryView mem, std::int64_t idx);
        DecodedInstruction cmpi(ConstExecutableMemoryView mem,
                                std::int64_t idx);
        DecodedInstruction cmpa(ConstExecutableMemoryView mem,
                                std::int64_t idx);
    } // namespace dec
} // namespace momiji
