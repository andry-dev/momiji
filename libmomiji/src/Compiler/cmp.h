#pragma once

#include "./Utils.h"
#include <momiji/Parser.h>

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void cmp(const momiji::ParsedInstruction& instr,
             const momiji::LabelInfo& labels,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData);

    void cmpa(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData);

    void cmpi(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData);

} // namespace momiji::enc
