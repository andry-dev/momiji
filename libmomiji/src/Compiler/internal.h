#pragma once

#include "./Utils.h"
#include <momiji/Parser.h>

#include <momiji/Utils.h>

namespace momiji::enc
{
    void hcf(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData);

    void breakpoint(const momiji::Instruction& instr,
                    OpcodeDescription& opcode,
                    std::array<AdditionalData, 2>& additionalData);

} // namespace momiji::enc
