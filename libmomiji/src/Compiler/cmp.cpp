#include "cmp.h"

#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"
#include <momiji/Utils.h>

namespace momiji::enc
{
    void cmp(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>& additionalData)
    {
        repr::Cmp bits;

        bits.datareg = instr.operands[1].value & 0b111;
        bits.size    = utils::to_val(instr.dataType) & 0b11;
        bits.srctype = utils::to_val(instr.operands[0].operandType) & 0b111;
        bits.srcmode = getCorrectOpMode(instr, 0);

        opcode.val = (bits.header << 12) | (bits.datareg << 9) |
                     (bits.padding << 8) | (bits.size << 6) |
                     (bits.srctype << 3) | (bits.srcmode);
    }

    void cmpa(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::CmpA bits;

        bits.addreg = instr.operands[1].value & 0b111;

        switch (instr.dataType)
        {
        case DataType::Byte:
            opcode.val = repr::Illegal {}.value;
            return;
            break;

        case DataType::Word:
            bits.size = 0;
            break;

        case DataType::Long:
            bits.size = 1;
            break;
        }

        bits.srctype = utils::to_val(instr.operands[0].operandType) & 0b111;
        bits.srcmode = getCorrectOpMode(instr, 0);

        opcode.val = (bits.header << 12) | (bits.addreg << 9) |
                     (bits.size << 8) | (bits.padding << 6) |
                     (bits.srctype << 3) | (bits.srcmode);
    }

    void cmpi(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        repr::CmpI bits;

        const std::uint8_t size = utils::to_val(instr.dataType);

        bits.size = size & 0b11;

        additionalData[0].cnt = tobyte[size];
        additionalData[0].val = instr.operands[0].value;

        bits.dsttype = utils::to_val(instr.operands[1].operandType) & 0b111;
        bits.dstmode = getCorrectOpMode(instr, 1);

        opcode.val = (bits.header << 8) | (bits.size << 6) |
                     (bits.dsttype << 3) | (bits.dstmode);
    }
} // namespace momiji::enc
