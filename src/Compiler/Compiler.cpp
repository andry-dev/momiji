#include <Compiler.h>

#include "../Instructions/OpCodes.h"
#include "./Utils.h"

#include <Utils.h>
#include <iostream>

#include "add.h"
#include "and.h"
#include "bcc.h"
#include "bra.h"
#include "cmp.h"
#include "div.h"
#include "exg.h"
#include "internal.h"
#include "jmp.h"
#include "move.h"
#include "mul.h"
#include "or.h"
#include "sub.h"
#include "swap.h"
#include "tst.h"
#include "xor.h"

namespace momiji
{
    ExecutableMemory
    compile(const std::vector<momiji::Instruction>& instructions)
    {
        ExecutableMemory memory;

        for (const auto& instr : instructions)
        {
            OpcodeDescription opcode;
            std::array<AdditionalData, 2> additional_data = { { { 0, 0 },
                                                                { 0, 0 } } };

            switch (instr.instructionType)
            {
            case InstructionType::Move:
            case InstructionType::MoveAddress:
                momiji::move(instr, opcode, additional_data);
                break;

            case InstructionType::Add:
                momiji::add(instr, opcode, additional_data);
                break;

            case InstructionType::AddA:
                momiji::adda(instr, opcode, additional_data);
                break;

            case InstructionType::AddI:
                momiji::addi(instr, opcode, additional_data);
                break;

            case InstructionType::Sub:
                momiji::sub(instr, opcode, additional_data);
                break;

            case InstructionType::SubA:
                momiji::suba(instr, opcode, additional_data);
                break;

            case InstructionType::SubI:
                momiji::subi(instr, opcode, additional_data);
                break;

            case InstructionType::SignedDiv:
                momiji::divs(instr, opcode, additional_data);
                break;

            case InstructionType::UnsignedDiv:
                momiji::divu(instr, opcode, additional_data);
                break;

            case InstructionType::SignedMul:
                momiji::muls(instr, opcode, additional_data);
                break;

            case InstructionType::UnsignedMul:
                momiji::mulu(instr, opcode, additional_data);
                break;

            case InstructionType::Or:
                momiji::or_instr(instr, opcode, additional_data);
                break;

            case InstructionType::OrI:
                momiji::ori(instr, opcode, additional_data);
                break;

            case InstructionType::And:
                momiji::and_instr(instr, opcode, additional_data);
                break;

            case InstructionType::AndI:
                momiji::andi(instr, opcode, additional_data);
                break;

            case InstructionType::Xor:
                momiji::xor_instr(instr, opcode, additional_data);
                break;

            case InstructionType::XorI:
                momiji::xori(instr, opcode, additional_data);
                break;

            case InstructionType::Not:
                break;

            case InstructionType::Compare:
                momiji::cmp(instr, opcode, additional_data);
                break;

            case InstructionType::CompareA:
                momiji::cmpa(instr, opcode, additional_data);
                break;

            case InstructionType::CompareI:
                momiji::cmpi(instr, opcode, additional_data);
                break;

            case InstructionType::Tst:
                momiji::tst(instr, opcode, additional_data);
                break;

            case InstructionType::Jmp:
                momiji::jmp(instr, opcode, additional_data);
                break;

            case InstructionType::JmpSubroutine:
                break;

            case InstructionType::Branch:
                momiji::bra(instr, opcode, additional_data);
                break;

            case InstructionType::BranchCondition:
                momiji::bcc(instr, opcode, additional_data);
                break;

            case InstructionType::BranchSubroutine:
                break;

            case InstructionType::ReturnSubroutine:
                break;

            case InstructionType::Swap:
                momiji::swap(instr, opcode, additional_data);
                break;

            case InstructionType::Exchange:
                momiji::exg(instr, opcode, additional_data);
                break;

            case InstructionType::ArithmeticShiftLeft:
            case InstructionType::ArithmeticShiftRight:
            case InstructionType::LogicalShiftLeft:
            case InstructionType::LogicalShiftRight:
                break;

            case InstructionType::HaltCatchFire:
                momiji::hcf(instr, opcode, additional_data);
                break;

            case InstructionType::Breakpoint:
                momiji::breakpoint(instr, opcode, additional_data);
                break;

            case InstructionType::Declare:
                switch (instr.dataType)
                {
                case DataType::Byte:
                    for (const auto& x : instr.operands)
                    {
                        const std::uint8_t val = x.value & 0x0000'00FF;
                        memory.push8(val);
                    }
                    break;

                case DataType::Word:
                    for (const auto& x : instr.operands)
                    {
                        const std::uint16_t val = x.value & 0x0000'FFFF;
                        memory.push16(val);
                    }
                    break;

                case DataType::Long:
                    for (const auto& x : instr.operands)
                    {
                        const std::uint32_t val = x.value;
                        memory.push32(val);
                    }
                    break;
                }
                continue;
                break;

            default:
                std::cerr << utils::to_val(instr.instructionType)
                          << ": Instruction not implemented yet\n";
            }

            memory.push16(opcode.val);

            for (int i = 0; i < additional_data.size(); ++i)
            {
                switch (additional_data[i].cnt)
                {
                case 1:
                {
                    // Adding 1 byte to align to 16-bit
                    const std::uint16_t val = 0 | additional_data[i].arr8[0];
                    memory.push16(val);
                }
                break;

                case 2:
                    memory.push16(additional_data[i].arr16[0]);
                    break;

                case 4:
                    memory.push32(additional_data[i].val);
                    break;
                }
            }
        }

        return memory;
    }
} // namespace momiji
