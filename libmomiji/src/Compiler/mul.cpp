#include "mul.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void muls(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::MulS bits;

        bits.datareg = std::get<operands::DataRegister>(instr.operands[1]).reg;

        bits.srctype = getCorrectOpType(instr.operands[0]);
        bits.srcmode = getCorrectOpMode(instr.operands[0]);

        handleAdditionalData(instr, labels, additionalData);

        opcode.val = std::uint16_t((bits.header << 12) | (bits.datareg << 9) |
                                   (bits.padding << 6) | (bits.srctype << 3) |
                                   (bits.srcmode));
    }

    void mulu(const momiji::ParsedInstruction& instr,
              const momiji::LabelInfo& labels,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::MulU bits;

        bits.datareg = std::get<operands::DataRegister>(instr.operands[1]).reg;

        bits.srctype = getCorrectOpType(instr.operands[0]);
        bits.srcmode = getCorrectOpMode(instr.operands[0]);

        handleAdditionalData(instr, labels, additionalData);

        opcode.val = std::uint16_t((bits.header << 12) | (bits.datareg << 9) |
                                   (bits.padding << 6) | (bits.srctype << 3) |
                                   (bits.srcmode));
    }
} // namespace momiji::enc
