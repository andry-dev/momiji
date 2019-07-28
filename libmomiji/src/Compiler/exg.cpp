#include "exg.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void exg(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Exg bits;

        bits.datareg = instr.operands[0].value & 0b111;
        bits.addreg  = instr.operands[1].value & 0b111;

        if (instr.operands[0].operandType == OperandType::DataRegister &&
            instr.operands[1].operandType == OperandType::DataRegister)
        {
            bits.mode = 0b01000;
        }
        else if (instr.operands[0].operandType ==
                     OperandType::AddressRegister &&
                 instr.operands[1].operandType == OperandType::AddressRegister)
        {
            bits.mode = 0b01001;
        }
        else if (instr.operands[0].operandType == OperandType::DataRegister &&
                 instr.operands[1].operandType == OperandType::AddressRegister)
        {
            bits.mode = 0b10001;
        }
        else if (instr.operands[0].operandType ==
                     OperandType::AddressRegister &&
                 instr.operands[1].operandType == OperandType::DataRegister)
        {
            bits.addreg  = instr.operands[0].value & 0b111;
            bits.datareg = instr.operands[1].value & 0b111;
            bits.mode    = 0b10001;
        }

        opcode.val = (bits.header << 12) | (bits.datareg << 9) |
                     (bits.padding << 8) | (bits.mode << 3) | (bits.addreg);
    }
} // namespace momiji::enc
