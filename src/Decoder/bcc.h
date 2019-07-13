#pragma once

#include <Decoder.h>
#include <System.h>

namespace momiji::dec
{
    DecodedInstruction bcc(ExecutableMemoryView mem, std::uint64_t idx);
}
