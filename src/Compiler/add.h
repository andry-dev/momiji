#pragma once

#include "./Utils.h"
#include "../Instructions/OpCodes.h"
#include "../Instructions/Representations.h"

#include <Parser.h>
#include <Utils.h>

namespace momiji
{
    void add(const momiji::Instruction& instr,
             OpcodeDescription& opcode,
             std::array<AdditionalData, 2>&) {
        repr::Add bits;

        // add.* d*, *
        if (instr.operands[0].operandType == OperandType::DataRegister)
        {
            bits.datareg = (instr.operands[0].value & 0b111);
            bits.direction = 1;
        }
        // add.* *, d*
        else
        {
            bits.datareg = (instr.operands[1].value & 0b111);
            bits.direction = 0;
        }

        bits.othtype = utils::to_val(instr.operands[bits.direction].operandType);
        bits.othmode = getCorrectOpMode(instr, bits.direction);

        std::uint8_t size = utils::to_val(instr.dataType);
        bits.size = (size & 0b11);


        opcode.val =  (bits.header << 12)
                    | (bits.datareg << 9)
                    | (bits.direction << 8)
                    | (bits.size << 6)
                    | (bits.othtype << 3)
                    | (bits.othmode);
    }

    void adda(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>&)
    {

        repr::AddA bits;

        bits.addreg = instr.operands[1].value & 0b111;
        switch (instr.dataType)
        {
        case DataType::Byte:
            break;

        case DataType::Word:
            bits.size = 0;
            break;

        case DataType::Long:
            bits.size = 1;
            break;
        }

        bits.srctype = utils::to_val(instr.operands[0].operandType) & 0b111;
        bits.srcmode = getCorrectOpMode(instr, 0) & 0b111;

        opcode.val =  (bits.header << 12)
                    | (bits.addreg << 9)
                    | (bits.size << 8)
                    | (bits.padding << 6)
                    | (bits.srctype << 3)
                    | (bits.srcmode);
    }

    void addi(const momiji::Instruction& instr,
              OpcodeDescription& opcode,
              std::array<AdditionalData, 2>& additionalData)
    {
        const std::uint8_t size = utils::to_val(instr.dataType) & 0b111;

        repr::AddI bits;
        bits.size = size & 0b11;

        additionalData[0].cnt = tobyte[size];
        additionalData[0].val = instr.operands[0].value;

        bits.dsttype = utils::to_val(instr.operands[1].operandType) & 0b111;
        bits.dstmode = getCorrectOpMode(instr, 1) & 0b111;

        opcode.val =  (bits.header << 8)
                    | (bits.size << 6)
                    | (bits.dsttype << 3)
                    | (bits.dstmode);
    }

}
