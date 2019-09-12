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
    ExecutableMemory compile(const momiji::ParsingInfo& parsingInfo,
                             CompilerSettings /*settings*/)
    {
        ExecutableMemory memory;

        const auto& labels = parsingInfo.labels;

        for (const auto& instr : parsingInfo.instructions)
        {
            OpcodeDescription opcode;
            std::array<AdditionalData, 2> additional_data = { { { 0, 0 },
                                                                { 0, 0 } } };

            switch (instr.instructionType)
            {
            case InstructionType::Move:
                momiji::enc::move(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Add:
                momiji::enc::add(instr, labels, opcode, additional_data);
                break;

            case InstructionType::AddA:
                momiji::enc::adda(instr, labels, opcode, additional_data);
                break;

            case InstructionType::AddI:
                momiji::enc::addi(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Sub:
                momiji::enc::sub(instr, labels, opcode, additional_data);
                break;

            case InstructionType::SubA:
                momiji::enc::suba(instr, labels, opcode, additional_data);
                break;

            case InstructionType::SubI:
                momiji::enc::subi(instr, labels, opcode, additional_data);
                break;

            case InstructionType::SignedDiv:
                momiji::enc::divs(instr, labels, opcode, additional_data);
                break;

            case InstructionType::UnsignedDiv:
                momiji::enc::divu(instr, labels, opcode, additional_data);
                break;

            case InstructionType::SignedMul:
                momiji::enc::muls(instr, labels, opcode, additional_data);
                break;

            case InstructionType::UnsignedMul:
                momiji::enc::mulu(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Or:
                momiji::enc::or_instr(instr, labels, opcode, additional_data);
                break;

            case InstructionType::OrI:
                momiji::enc::ori(instr, labels, opcode, additional_data);
                break;

            case InstructionType::And:
                momiji::enc::and_instr(instr, labels, opcode, additional_data);
                break;

            case InstructionType::AndI:
                momiji::enc::andi(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Xor:
                momiji::enc::xor_instr(instr, labels, opcode, additional_data);
                break;

            case InstructionType::XorI:
                momiji::enc::xori(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Not:
                momiji::enc::not_instr(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Compare:
                momiji::enc::cmp(instr, labels, opcode, additional_data);
                break;

            case InstructionType::CompareA:
                momiji::enc::cmpa(instr, labels, opcode, additional_data);
                break;

            case InstructionType::CompareI:
                momiji::enc::cmpi(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Tst:
                momiji::enc::tst(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Jmp:
                momiji::enc::jmp(instr, labels, opcode, additional_data);
                break;

            case InstructionType::JmpSubroutine:
                momiji::enc::jsr(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Branch:
                momiji::enc::bra(instr, labels, opcode, additional_data);
                break;

            case InstructionType::BranchCondition:
                momiji::enc::bcc(instr, labels, opcode, additional_data);
                break;

            case InstructionType::BranchSubroutine:
                momiji::enc::bsr(instr, labels, opcode, additional_data);
                break;

            case InstructionType::ReturnSubroutine:
                momiji::enc::rts(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Swap:
                momiji::enc::swap(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Exchange:
                momiji::enc::exg(instr, labels, opcode, additional_data);
                break;

            case InstructionType::ArithmeticShiftRight:
                momiji::enc::any_shift<repr::RegAsd, repr::MemAsd>(
                    instr, labels, opcode, additional_data, 0);
                break;

            case InstructionType::ArithmeticShiftLeft:
                momiji::enc::any_shift<repr::RegAsd, repr::MemAsd>(
                    instr, labels, opcode, additional_data, 1);
                break;

            case InstructionType::LogicalShiftRight:
                momiji::enc::any_shift<repr::RegLsd, repr::MemLsd>(
                    instr, labels, opcode, additional_data, 0);
                break;

            case InstructionType::LogicalShiftLeft:
                momiji::enc::any_shift<repr::RegLsd, repr::MemLsd>(
                    instr, labels, opcode, additional_data, 1);
                break;

            case InstructionType::HaltCatchFire:
                momiji::enc::hcf(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Breakpoint:
                momiji::enc::breakpoint(instr, labels, opcode, additional_data);
                break;

            case InstructionType::Declare:
                switch (instr.dataType)
                {
                case DataType::Byte:
                    for (const auto& x : instr.operands)
                    {
                        const std::uint8_t val =
                            extractASTValue(x, parsingInfo.labels) &
                            0x0000'00FF;
                        memory.push8(val);
                    }
                    break;

                case DataType::Word:
                    for (const auto& x : instr.operands)
                    {
                        const std::uint16_t val =
                            extractASTValue(x, parsingInfo.labels) &
                            0x0000'FFFF;
                        memory.push16(val);
                    }
                    break;

                case DataType::Long:
                    for (const auto& x : instr.operands)
                    {
                        const auto val = std::uint32_t(
                            extractASTValue(x, parsingInfo.labels));
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
                [[fallthrough]];
            case InstructionType::CodeMarker:
                [[fallthrough]];
            case InstructionType::DataMarker:
                /* default: */

                // Ignore these instructions for now
                continue;
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
