#include "not.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Parser.h>

namespace momiji::enc
{
    void not_instr(const momiji::ParsedInstruction& instr,
                   const momiji::LabelInfo& labels,
                   OpcodeDescription& /*opcode*/,
                   std::array<AdditionalData, 2>& additionalData)
    {
        repr::Not bits;

        bits.regtype = getCorrectOpType(instr.operands[0]);
        bits.regmode = getCorrectOpMode(instr.operands[0]);

        bits.size = utils::to_val(instr.dataType) & 0b11;

        handleAdditionalData(instr, labels, additionalData);
    }
} // namespace momiji::enc
