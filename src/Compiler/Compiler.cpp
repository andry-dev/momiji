#include <Compiler.h>

#include "../Instructions/OpCodes.h"
#include "./Utils.h"

#include <Utils.h>
#include <iostream>

#include "move.h"
#include "add.h"
#include "sub.h"
#include "div.h"
#include "or.h"
#include "and.h"
#include "cmp.h"
#include "mul.h"
#include "exg.h"
#include "bra.h"
#include "bcc.h"
#include "jmp.h"
#include "swap.h"

namespace momiji
{
    std::vector<std::uint16_t>
    compile(const std::vector<momiji::Instruction>& instructions)
    {
        std::vector<std::uint16_t> memory;

        for (const auto& instr : instructions)
        {
            OpcodeDescription opcode;
            std::array<AdditionalData, 2> additional_data = {{
                { 0, 0 },
                { 0, 0 }
            }};

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

            case InstructionType::SignedMul:
                momiji::muls(instr, memory, opcode, additional_data);
                break;

            case InstructionType::UnsignedMul:
                momiji::mulu(instr, memory, opcode, additional_data);
                break;

            case InstructionType::Or:
                momiji::or_instr(instr, memory, opcode, additional_data);
                break;

            case InstructionType::OrI:
                momiji::ori(instr, memory, opcode, additional_data);
                break;

            case InstructionType::And:
                momiji::and_instr(instr, memory, opcode, additional_data);
                break;

            case InstructionType::AndI:
                momiji::andi(instr, memory, opcode, additional_data);
                break;

            case InstructionType::Xor:
                break;

            case InstructionType::XorI:
                break;

            case InstructionType::Not:
                break;

            case InstructionType::Compare:
                momiji::cmp(instr, memory, opcode, additional_data);
                break;

            case InstructionType::CompareA:
                momiji::cmpa(instr, memory, opcode, additional_data);
                break;

            case InstructionType::CompareI:
                momiji::cmpi(instr, memory, opcode, additional_data);
                break;

            case InstructionType::Branch:
                momiji::bra(instr, memory, opcode, additional_data);
                break;

            case InstructionType::BranchCondition:
                momiji::bcc(instr, memory, opcode, additional_data);
                break;

            case InstructionType::Exchange:
                momiji::exg(instr, memory, opcode, additional_data);
                break;

            case InstructionType::Swap:
                momiji::swap(instr, memory, opcode, additional_data);
                break;

            case InstructionType::Jmp:
                momiji::jmp(instr, memory, opcode, additional_data);
                break;

            case InstructionType::Tst:
                break;

            case InstructionType::ArithmeticShiftLeft:
            case InstructionType::ArithmeticShiftRight:
            case InstructionType::LogicalShiftLeft:
            case InstructionType::LogicalShiftRight:
                break;
            }

            memory.push_back(opcode.val);

            for (int i = 0; i < additional_data.size(); ++i)
            {
                switch (additional_data[i].cnt)
                {
                case 1: {
                    // Adding 1 byte to align to 16-bit
                    const std::uint16_t val = 0 | additional_data[i].arr8[0];
                    memory.push_back(val);
                    } break;
                case 2:
                    memory.push_back(additional_data[i].arr16[0]);
                    break;

                case 4:
                    memory.push_back(additional_data[i].arr16[1]);
                    memory.push_back(additional_data[i].arr16[0]);
                    break;
                }
            }
        }

        return memory;
    }
}
