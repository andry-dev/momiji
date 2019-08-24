#pragma once

#include <momiji/Decoder.h>

namespace momiji::dec
{
    DecodedInstruction mem_rod(ConstExecutableMemoryView mem, std::int64_t idx);
    DecodedInstruction reg_rod(ConstExecutableMemoryView mem, std::int64_t idx);
} // namespace momiji::dec
