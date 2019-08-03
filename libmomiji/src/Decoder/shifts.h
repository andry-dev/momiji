#pragma once

#include "../Instructions/Representations.h"
#include "../Instructions/shifts.h"
#include "./Utils.h"
#include <momiji/Decoder.h>

namespace momiji::dec
{
    namespace details
    {
        struct ArithmeticShiftMetaType
        {
            using MemRepr = repr::MemAsd;
            using RegRepr = repr::RegAsd;

            static momiji::DecodedInstructionFn RightFn()
            {
                return &instr::shift<instr::details::ArithShiftRight>;
            }

            static momiji::InstructionString RightStr()
            {
                return "asr ";
            }

            static momiji::DecodedInstructionFn LeftFn()
            {
                return &instr::shift<instr::details::ArithShiftLeft>;
            }

            static momiji::InstructionString LeftStr()
            {
                return "asl ";
            }
        };

        struct LogicalShiftMetaType
        {
            using MemRepr = repr::MemLsd;
            using RegRepr = repr::RegLsd;

            static momiji::DecodedInstructionFn RightFn()
            {
                return &instr::shift<instr::details::LogicalShiftRight>;
            }

            static momiji::InstructionString RightStr()
            {
                return "lsr ";
            }

            static momiji::DecodedInstructionFn LeftFn()
            {
                return &instr::shift<instr::details::LogicalShiftLeft>;
            }

            static momiji::InstructionString LeftStr()
            {
                return "lsl ";
            }
        };

    } // namespace details

    template <typename ShiftType>
    DecodedInstruction any_mem_shift(ExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        const std::uint16_t val = mem.read16(idx);

        typename ShiftType::MemRepr bits;

        bits.direction = (val & 0b00000001'00000000) >> 8;
        bits.regtype   = (val & 0b00000000'00111000) >> 3;
        bits.regmode   = (val & 0b00000000'00000111);

        ret.data.operandType[0] = static_cast<OperandType>(bits.regtype);
        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.regmode);

        ret.data.operandType[1] = OperandType::Address;

        if (bits.direction == 0)
        {
            ret.exec   = ShiftType::RightFn();
            ret.string = ShiftType::RightStr() +
                         momiji::opToString(ret.data, 0, mem, idx);
        }
        else
        {
            ret.exec   = ShiftType::LeftFn();
            ret.string = ShiftType::LeftStr() +
                         momiji::opToString(ret.data, 0, mem, idx);
        }

        return ret;
    }

    template <typename ShiftType>
    DecodedInstruction any_reg_shift(ExecutableMemoryView mem, std::int64_t idx)
    {
        DecodedInstruction ret;

        const std::uint16_t val = mem.read16(idx);

        typename ShiftType::RegRepr bits;

        bits.rotation  = (val & 0b00001110'00000000) >> 9;
        bits.direction = (val & 0b00000001'00000000) >> 8;
        bits.size      = (val & 0b00000000'11000000) >> 6;
        bits.rotmode   = (val & 0b00000000'00100000) >> 5;
        bits.datareg   = (val & 0b00000000'00000111);

        if (bits.direction == 0)
        {
            ret.exec   = ShiftType::RightFn();
            ret.string = ShiftType::RightStr();
        }
        else
        {
            ret.exec   = ShiftType::LeftFn();
            ret.string = ShiftType::LeftStr();
        }

        ret.data.addressingMode[0] =
            static_cast<SpecialAddressingMode>(bits.rotation);

        if (bits.rotmode == 0)
        {
            ret.data.operandType[0] = OperandType::Immediate;
            ret.string += "#" + std::to_string(bits.rotation);
        }
        else
        {
            ret.data.operandType[0] = OperandType::DataRegister;
            ret.string += "d" + std::to_string(bits.rotation);
        }

        ret.data.operandType[1] = OperandType::DataRegister;
        ret.data.addressingMode[1] =
            static_cast<SpecialAddressingMode>(bits.datareg);

        ret.string += ", d" + std::to_string(bits.datareg);

        return ret;
    }
} // namespace momiji::dec
