#pragma once

#include "./Utils.h"
#include <momiji/Parser.h>

namespace momiji::enc
{
    void or_instr(const momiji::Instruction& instr,
                  OpcodeDescription& opcode,
                  std::array<AdditionalData, 2>& additionalData);

    void ori(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData);

} // namespace momiji::enc
