#pragma once

#include "./Utils.h"
#include <momiji/Parser.h>

namespace momiji::enc
{
    void add(const momiji::ParsedInstruction& instr,
             const momiji::LabelInfo& labels,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData);

    void adda(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData);

    void addi(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData);

} // namespace momiji::enc
