#include "mul.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void muls(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::MulS bits;

        bits.datareg = instr.operands[1].value & 0b111;

        bits.srctype = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = getCorrectOpMode(instr, 0);

        handleAdditionalData(instr, additionalData);

        opcode.val = std::uint16_t((bits.header << 12) | (bits.datareg << 9) |
                                   (bits.padding << 6) | (bits.srctype << 3) |
                                   (bits.srcmode));
    }

    void mulu(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::MulU bits;

        bits.datareg = instr.operands[1].value & 0b111;

        bits.srctype = utils::to_val(instr.operands[0].operandType);
        bits.srcmode = getCorrectOpMode(instr, 0);

        handleAdditionalData(instr, additionalData);

        opcode.val = std::uint16_t((bits.header << 12) | (bits.datareg << 9) |
                                   (bits.padding << 6) | (bits.srctype << 3) |
                                   (bits.srcmode));
    }
} // namespace momiji::enc
