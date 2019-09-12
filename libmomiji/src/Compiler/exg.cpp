#include "exg.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void exg(const momiji::ParsedInstruction& instr,
             const momiji::LabelInfo& /*labels*/,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& /*additionalData*/)
    {
        repr::Exg bits;

        bits.datareg = std::uint16_t(extractRegister(instr.operands[0]));
        bits.addreg  = std::uint16_t(extractRegister(instr.operands[1]));

        if (matchOperand<operands::DataRegister>(instr.operands[0]) &&
            matchOperand<operands::DataRegister>(instr.operands[1]))
        {
            bits.mode = 0b01000;
        }
        else if (matchOperand<operands::AddressRegister>(instr.operands[0]) &&
                 matchOperand<operands::AddressRegister>(instr.operands[1]))
        {
            bits.mode = 0b01001;
        }
        else if (matchOperand<operands::DataRegister>(instr.operands[0]) &&
                 matchOperand<operands::AddressRegister>(instr.operands[1]))
        {
            bits.mode = 0b10001;
        }
        else if (matchOperand<operands::AddressRegister>(instr.operands[0]) &&
                 matchOperand<operands::DataRegister>(instr.operands[1]))
        {
            bits.mode = 0b10001;

            bits.datareg = std::uint16_t(extractRegister(instr.operands[1]));
            bits.addreg  = std::uint16_t(extractRegister(instr.operands[0]));
        }

        opcode.val = std::uint16_t((bits.header << 12) | (bits.datareg << 9) |
                                   (bits.padding << 8) | (bits.mode << 3) |
                                   (bits.addreg));
    }
} // namespace momiji::enc
