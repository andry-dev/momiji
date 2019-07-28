#pragma once

#include "./Utils.h"
#include <momiji/Parser.h>

namespace momiji::enc
{
    void divs(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData);

    void divu(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData);

} // namespace momiji::enc
