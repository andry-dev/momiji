#include <Compiler.h>

#include "../Instructions/OpCodes.h"
#include "./Utils.h"

#include <Utils.h>
#include <iostream>

#include "./move.h"
#include "./add.h"
#include "./sub.h"
#include "./div.h"

namespace momiji
{
    std::vector<std::uint16_t>
    compile(const std::vector<momiji::Instruction>& instructions)
    {
        std::vector<std::uint16_t> memory;

        for (const auto& instr : instructions)
        {
            OpcodeDescription opcode;
            AdditionalData additional_data;
            additional_data.val = 0;

            switch (instr.instructionType)
            {
            case InstructionType::Move:
            case InstructionType::MoveAddress:
                momiji::move(instr, memory, opcode, additional_data);
                break;

            case InstructionType::Add:
                momiji::add(instr, memory, opcode, additional_data);
                break;

            case InstructionType::AddA:
                momiji::adda(instr, memory, opcode, additional_data);
                break;

            case InstructionType::AddI:
                momiji::addi(instr, memory, opcode, additional_data);
                break;

            case InstructionType::Sub:
                momiji::sub(instr, memory, opcode, additional_data);
                break;

            case InstructionType::SubA:
                momiji::suba(instr, memory, opcode, additional_data);
                break;

            case InstructionType::SubI:
                momiji::subi(instr, memory, opcode, additional_data);
                break;

            case InstructionType::SignedDiv:
                momiji::divs(instr, memory, opcode, additional_data);
                break;

            case InstructionType::UnsignedDiv:
                momiji::divu(instr, memory, opcode, additional_data);
                break;

            default:
                break;
            }

            memory.push_back(opcode.val);

            switch (additional_data.cnt)
            {
            case 1: {
                // Adding 1 byte to align to 16-bit
                std::uint16_t val = 0 | additional_data.arr8[0];
                memory.push_back(val);
                } break;
            case 2:
                memory.push_back(additional_data.arr16[0]);
                break;

            case 4:
                memory.push_back(additional_data.arr16[1]);
                memory.push_back(additional_data.arr16[0]);
                break;
            }
        }

        return memory;
    }
}
