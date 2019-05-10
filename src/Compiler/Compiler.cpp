#include <Compiler.h>

#include "../Instructions/OpCodes.h"
#include "./Utils.h"

#include <Utils.h>
#include <iostream>

#include "./move.h"
#include "./add.h"
#include "./sub.h"

namespace momiji
{
    std::vector<std::uint8_t>
    compile(const std::vector<momiji::Instruction>& instructions)
    {
        std::vector<std::uint8_t> memory;

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

            default:
                break;
            }

            memory.push_back(opcode.higher);
            memory.push_back(opcode.lower);

            std::cout << "Adding bytes: "
                << std::to_string(additional_data.cnt)
                << " - "
                << additional_data.val << '\n';

            for (int i = 0; i < additional_data.cnt; ++i)
            {
                memory.push_back(additional_data.arr8[i]);
            }
        }

        return memory;
    }
}
