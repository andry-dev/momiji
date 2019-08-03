#include <Compiler.h>

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
#include "not.h"
#include "or.h"
#include "rod.h"
#include "roxd.h"
#include "rts.h"
#include "shifts.h"
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
                momiji::enc::move(instr, opcode, additional_data);
                break;

            case InstructionType::Add:
                momiji::enc::add(instr, opcode, additional_data);
                break;

            case InstructionType::AddA:
                momiji::enc::adda(instr, opcode, additional_data);
                break;

            case InstructionType::AddI:
                momiji::enc::addi(instr, opcode, additional_data);
                break;

            case InstructionType::Sub:
                momiji::enc::sub(instr, opcode, additional_data);
                break;

            case InstructionType::SubA:
                momiji::enc::suba(instr, opcode, additional_data);
                break;

            case InstructionType::SubI:
                momiji::enc::subi(instr, opcode, additional_data);
                break;

            case InstructionType::SignedDiv:
                momiji::enc::divs(instr, opcode, additional_data);
                break;

            case InstructionType::UnsignedDiv:
                momiji::enc::divu(instr, opcode, additional_data);
                break;

            case InstructionType::SignedMul:
                momiji::enc::muls(instr, opcode, additional_data);
                break;

            case InstructionType::UnsignedMul:
                momiji::enc::mulu(instr, opcode, additional_data);
                break;

            case InstructionType::Or:
                momiji::enc::or_instr(instr, opcode, additional_data);
                break;

            case InstructionType::OrI:
                momiji::enc::ori(instr, opcode, additional_data);
                break;

            case InstructionType::And:
                momiji::enc::and_instr(instr, opcode, additional_data);
                break;

            case InstructionType::AndI:
                momiji::enc::andi(instr, opcode, additional_data);
                break;

            case InstructionType::Xor:
                momiji::enc::xor_instr(instr, opcode, additional_data);
                break;

            case InstructionType::XorI:
                momiji::enc::xori(instr, opcode, additional_data);
                break;

            case InstructionType::Not:
                momiji::enc::not_instr(instr, opcode, additional_data);
                break;

            case InstructionType::Compare:
                momiji::enc::cmp(instr, opcode, additional_data);
                break;

            case InstructionType::CompareA:
                momiji::enc::cmpa(instr, opcode, additional_data);
                break;

            case InstructionType::CompareI:
                momiji::enc::cmpi(instr, opcode, additional_data);
                break;

            case InstructionType::Tst:
                momiji::enc::tst(instr, opcode, additional_data);
                break;

            case InstructionType::Jmp:
                momiji::enc::jmp(instr, opcode, additional_data);
                break;

            case InstructionType::JmpSubroutine:
                momiji::enc::jsr(instr, opcode, additional_data);
                break;

            case InstructionType::Branch:
                momiji::enc::bra(instr, opcode, additional_data);
                break;

            case InstructionType::BranchCondition:
                momiji::enc::bcc(instr, opcode, additional_data);
                break;

            case InstructionType::BranchSubroutine:
                momiji::enc::bsr(instr, opcode, additional_data);
                break;

            case InstructionType::ReturnSubroutine:
                momiji::enc::rts(instr, opcode, additional_data);
                break;

            case InstructionType::Swap:
                momiji::enc::swap(instr, opcode, additional_data);
                break;

            case InstructionType::Exchange:
                momiji::enc::exg(instr, opcode, additional_data);
                break;

            case InstructionType::ArithmeticShiftRight:
                momiji::enc::any_shift<repr::RegAsd, repr::MemAsd>(
                    instr, opcode, additional_data, 0);
                break;

            case InstructionType::ArithmeticShiftLeft:
                momiji::enc::any_shift<repr::RegAsd, repr::MemAsd>(
                    instr, opcode, additional_data, 1);
                break;

            case InstructionType::LogicalShiftRight:
                momiji::enc::any_shift<repr::RegLsd, repr::MemLsd>(
                    instr, opcode, additional_data, 0);
                break;

            case InstructionType::LogicalShiftLeft:
                momiji::enc::any_shift<repr::RegLsd, repr::MemLsd>(
                    instr, opcode, additional_data, 1);
                break;

            case InstructionType::HaltCatchFire:
                momiji::enc::hcf(instr, opcode, additional_data);
                break;

            case InstructionType::Breakpoint:
                momiji::enc::breakpoint(instr, opcode, additional_data);
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
                        const std::uint32_t val = std::uint32_t(x.value);
                        memory.push32(val);
                    }
                    break;
                }
                continue;

            case InstructionType::AddQ:
                [[fallthrough]];
            case InstructionType::SubQ:
                [[fallthrough]];
            case InstructionType::MoveQuick:
                [[fallthrough]];
            case InstructionType::Neg:
                [[fallthrough]];
            case InstructionType::Nop:
                [[fallthrough]];
            case InstructionType::Illegal:
                /* default: */
                std::cerr << utils::to_val(instr.instructionType)
                          << ": Instruction not implemented yet\n";
            }

            memory.push16(opcode.val);

            for (std::uint32_t i = 0; i < additional_data.size(); ++i)
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
