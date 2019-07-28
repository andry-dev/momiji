#pragma once

#include "./Utils.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void not_instr(const momiji::Instruction& instr,
                   OpcodeDescription& opcode,
                   std::array<AdditionalData, 2>& additionalData);

} // namespace momiji::enc