#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::dec
{
    DecodedInstruction add(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction adda(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction addi(ConstExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
