#pragma once

#include "System.h"
#include "Parser.h"

#include <array>

namespace momiji
{
    namespace op_impl
    {
        inline momiji::System move8(momiji::System sys, const momiji::Instruction& instr)
        {
            union
            {
                std::int8_t arr[4];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate: {
                pun.val = instr.operands[0].value & 0x000000FF;
            } break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | (pun.arr[0] & 0x000000FF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | (pun.arr[0] & 0x000000FF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System move16(momiji::System sys, const momiji::Instruction& instr)
        {
            union
            {
                std::int16_t arr[2];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | (pun.arr[0] & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | (pun.arr[0] & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }



            return sys;

        }

        inline momiji::System move32(momiji::System sys, const momiji::Instruction& instr)
        {
            std::int32_t tmp = 0;
            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                tmp = instr.operands[0].value;
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    tmp = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Data:
                    tmp = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Special:
                    break;
                }
            }

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System moveq(momiji::System sys, const momiji::Instruction& instr)
        {
            std::int8_t trunc = instr.operands[0].value & 0x000000FF;
            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | trunc;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;

                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | trunc;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System add8(momiji::System sys, const momiji::Instruction& instr)
        {
            union
            {
                std::int8_t arr[4];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp + pun.arr[0]) & 0x000000FF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp + pun.arr[0]) & 0x000000FF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System add16(momiji::System sys, const momiji::Instruction& instr)
        {
            union
            {
                std::int16_t arr[2];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp + pun.arr[0]) & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp + pun.arr[0]) & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System add32(momiji::System sys, const momiji::Instruction& instr)
        {
            std::int32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                val = instr.operands[0].value;
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = tmp + val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = tmp + val;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System sub8(momiji::System sys, const momiji::Instruction& instr)
        {
            union
            {
                std::int8_t arr[4];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp - pun.arr[0]) & 0x000000FF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp - pun.arr[0]) & 0x000000FF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System sub16(momiji::System sys, const momiji::Instruction& instr)
        {
            union
            {
                std::int16_t arr[2];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp - pun.arr[0]) & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp - pun.arr[0]) & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System sub32(momiji::System sys, const momiji::Instruction& instr)
        {
            std::int32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                val = instr.operands[0].value;
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = tmp - val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = tmp - val;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System muls(momiji::System sys, const momiji::Instruction& instr)
        {
            std::int32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                val = instr.operands[0].value;
                break;
            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;
                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;
                case RegisterType::Special:
                    break;
                }
                break;
            }

            bool signedness = val < 0;

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp *= val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp *= val;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System mulu(momiji::System sys, const momiji::Instruction& instr)
        {
            std::uint32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                val = static_cast<std::uint32_t>(
                        instr.operands[0].value & 0x0000FFFF);
                break;
            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = static_cast<std::uint32_t>(
                            sys.cpu.addressRegisters[instr.operands[0].value].value);
                    break;
                case RegisterType::Data:
                    val = static_cast<std::uint32_t>(
                            sys.cpu.dataRegisters[instr.operands[0].value].value);
                    break;
                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::uint32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value & 0x0000FFFF;
                tmp = tmp * val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value & 0x0000FFFF;
                tmp = tmp * val;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System divs(momiji::System sys, const momiji::Instruction& instr)
        {
            std::int32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                val = instr.operands[0].value;
                break;
            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;
                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;
                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;
            std::int32_t quotient = 0;
            std::int32_t reminder = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System divu(momiji::System sys, const momiji::Instruction& instr)
        {
            std::uint32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                val = instr.operands[0].value;
                break;
            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;
                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;
                case RegisterType::Special:
                    break;
                }
                break;
            }

            std::uint32_t tmp = 0;
            std::uint32_t quotient = 0;
            std::uint32_t reminder = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Special:
                break;
            }

            return sys;
        }

        inline momiji::System swap(momiji::System sys, const momiji::Instruction& instr)
        {
            return sys;
        }

        inline momiji::System exg(momiji::System sys, const momiji::Instruction& instr)
        {
            return sys;
        }

        inline momiji::System bra(momiji::System sys, const momiji::Instruction& instr)
        {
            sys.cpu.programCounter.value = instr.operands[0].value;
            return sys;
        }

        inline momiji::System jmp(momiji::System sys, const momiji::Instruction& instr)
        {
            return sys;
        }


        constexpr std::array<momiji::instr_fn_t, 3> move{ move8, move16, move32 };
        constexpr std::array<momiji::instr_fn_t, 3> add{ add8, add16, add32 };
        constexpr std::array<momiji::instr_fn_t, 3> sub{ sub8, sub16, sub32 };
    }
}
