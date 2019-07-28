#include "not.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Parser.h>

namespace momiji::enc
{
    void not_instr(const momiji::Instruction& instr,
                   OpcodeDescription& opcode,
                   std::array<AdditionalData, 2>& additionalData)
    {
        repr::Not bits;

        bits.regtype = utils::to_val(instr.operands[0].operandType);
        bits.regmode = getCorrectOpMode(instr, 0);

        bits.size = utils::to_val(instr.dataType) & 0b11;

        handleAdditionalData(instr, additionalData);
    }
} // namespace momiji::enc
