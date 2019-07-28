#pragma once

#include "./Utils.h"
#include <momiji/Parser.h>

namespace momiji::enc
{
    void xor_instr(const momiji::Instruction& instr,
                   OpcodeDescription& opcode,
                   std::array<AdditionalData, 2>& additionalData);

    void xori(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData);
} // namespace momiji::enc
