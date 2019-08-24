#pragma once

#include <momiji/Decoder.h>

namespace momiji::dec
{
    DecodedInstruction rts(ConstExecutableMemoryView mem, std::int64_t idx);
}
