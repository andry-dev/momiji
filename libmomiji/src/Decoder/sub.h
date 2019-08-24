#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::dec
{
    DecodedInstruction sub(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction suba(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction subi(ConstExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
