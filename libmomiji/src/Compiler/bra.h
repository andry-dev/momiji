#pragma once

#include "./Utils.h"
#include <momiji/Parser.h>

namespace momiji::enc
{
    void bra(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData);

    void bsr(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData);

} // namespace momiji::enc
