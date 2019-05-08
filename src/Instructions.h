#pragma once

#include "System.h"
#include "Parser.h"
#include <Utils.h>

#include <array>
#include <iostream>

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
            case OperandType::Immediate:
                pun.val = instr.operands[0].value & 0x000000FF;
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

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
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

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
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

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
                break;
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
            std::int32_t tmp = instr.operands[1].value;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[tmp].value;

                tmp = (tmp & 0xFFFFFF00) | trunc;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;

                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[tmp].value;

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

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
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

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
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

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
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

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
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

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
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
            std::int32_t val = instr.operands[0].value;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[val].value;
                    break;

                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[val].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
                break;
            }

            std::int32_t tmp = instr.operands[1].value;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[tmp].value;
                tmp = tmp - val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[tmp].value;
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
            std::int32_t val = instr.operands[0].value;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                break;
            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[val].value;
                    break;
                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[val].value;
                    break;
                case RegisterType::Special:
                    break;
                }
                break;

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
                break;
            }

            val = utils::sign_extend<std::int16_t>(val);

            std::int32_t tmp = instr.operands[1].value;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[tmp].value;
                tmp = utils::sign_extend<std::int16_t>(tmp);
                tmp *= val;
                sys.cpu.addressRegisters[tmp].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = utils::sign_extend<std::int16_t>(tmp);
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
            std::uint32_t val = instr.operands[0].value;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                break;
            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[val].value;
                    break;
                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[val].value;
                    break;
                case RegisterType::Special:
                    break;
                }
                break;

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
                break;
            }

            val = val & 0x0000FFFF;

            std::uint32_t tmp = instr.operands[1].value;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[tmp].value & 0x0000FFFF;
                tmp = tmp * val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[tmp].value & 0x0000FFFF;
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
            std::int32_t val = instr.operands[0].value;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                break;
            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[val].value;
                    break;
                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[val].value;
                    break;
                case RegisterType::Special:
                    break;
                }
                break;

            case OperandType::Label: [[fallthrough]];
            case OperandType::Address:
                break;
            }

            val = utils::sign_extend<std::int16_t>(val);

            std::int32_t tmp = instr.operands[1].value;
            std::int32_t quotient = 0;
            std::int32_t reminder = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[tmp].value;
                tmp = utils::sign_extend<std::int16_t>(tmp);
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[tmp].value;
                tmp = utils::sign_extend<std::int16_t>(tmp);
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
            std::uint32_t val = instr.operands[0].value;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                break;
            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Address:
                    val = sys.cpu.addressRegisters[val].value;
                    break;
                case RegisterType::Data:
                    val = sys.cpu.dataRegisters[val].value;
                    break;
                case RegisterType::Special:
                    break;
                }
                break;
            }

            val = val & 0x0000FFFF;

            std::uint32_t tmp = instr.operands[1].value;
            std::uint32_t quotient = 0;
            std::uint32_t reminder = 0;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Address:
                tmp = sys.cpu.addressRegisters[tmp].value & 0x0000FFFF;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case RegisterType::Data:
                tmp = sys.cpu.dataRegisters[tmp].value & 0x0000FFFF;
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
            const auto val = instr.operands[0].value;
            std::int32_t* reg = nullptr; // oof

            switch (instr.operands[0].registerType)
            {
            case RegisterType::Data:
                reg = &sys.cpu.dataRegisters[val].value;
                break;

            case RegisterType::Address:
                reg = &sys.cpu.addressRegisters[val].value;
                break;

            case RegisterType::Special:
                break;
            }

            std::int32_t low = (*reg & 0x0000FFFF) << 16;
            std::int32_t high = (*reg & 0xFFFF0000) >> 16;

            *reg = (low | high);

            return sys;
        }

        inline momiji::System exg(momiji::System sys, const momiji::Instruction& instr)
        {
            return sys;
        }

        inline momiji::System cmp8(momiji::System sys, const momiji::Instruction& instr)
        {
            std::int32_t src = instr.operands[0].value;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Data:
                    src = sys.cpu.dataRegisters[src].value;
                    break;

                case RegisterType::Address:
                    src = sys.cpu.addressRegisters[src].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;

            default:
                break;
            }

            src = utils::sign_extend<std::int8_t>(src);

            std::int32_t dst = instr.operands[1].value;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Data:
                dst = sys.cpu.dataRegisters[dst].value;
                break;

            case RegisterType::Address:
                dst = sys.cpu.addressRegisters[dst].value;
                break;

            case RegisterType::Special:
                break;
            }

            dst = utils::sign_extend<std::int8_t>(dst);

            const std::int32_t res = dst - src;

            sys.cpu.statusRegister.negative = (res < 0) ? 1 : 0;
            sys.cpu.statusRegister.zero = (res == 0) ? 1 : 0;
            sys.cpu.statusRegister.overflow = utils::sub_overflow(dst, src) ? 1 : 0;

            return sys;
        }

        inline momiji::System cmp16(momiji::System sys, const momiji::Instruction& instr)
        {
            std::int32_t src = instr.operands[0].value;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Data:
                    src = sys.cpu.dataRegisters[src].value;
                    break;

                case RegisterType::Address:
                    src = sys.cpu.addressRegisters[src].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;

            default:
                break;
            }

            src = utils::sign_extend<std::int16_t>(src);

            std::int32_t dst = instr.operands[1].value;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Data:
                dst = sys.cpu.dataRegisters[dst].value;
                break;

            case RegisterType::Address:
                dst = sys.cpu.addressRegisters[dst].value;
                break;

            case RegisterType::Special:
                break;
            }

            dst = utils::sign_extend<std::int16_t>(dst);

            const std::int32_t res = dst - src;

            sys.cpu.statusRegister.negative = (res < 0) ? 1 : 0;
            sys.cpu.statusRegister.zero = (res == 0) ? 1 : 0;
            sys.cpu.statusRegister.overflow = utils::sub_overflow(dst, src) ? 1 : 0;

            return sys;
        }

        inline momiji::System cmp32(momiji::System sys, const momiji::Instruction& instr)
        {
            std::int32_t src = instr.operands[0].value;

            switch (instr.operands[0].operandType)
            {
            case OperandType::Immediate:
                break;

            case OperandType::Register:
                switch (instr.operands[0].registerType)
                {
                case RegisterType::Data:
                    src = sys.cpu.dataRegisters[src].value;
                    break;

                case RegisterType::Address:
                    src = sys.cpu.addressRegisters[src].value;
                    break;

                case RegisterType::Special:
                    break;
                }
                break;

            case OperandType::Address: [[fallthrough]];
            case OperandType::Label:
                break;
            }

            std::int32_t dst = instr.operands[1].value;

            switch (instr.operands[1].registerType)
            {
            case RegisterType::Data:
                dst = sys.cpu.dataRegisters[dst].value;
                break;

            case RegisterType::Address:
                dst = sys.cpu.addressRegisters[dst].value;
                break;

            case RegisterType::Special:
                break;
            }

            const std::int32_t res = dst - src;

            sys.cpu.statusRegister.negative = (res < 0) ? 1 : 0;
            sys.cpu.statusRegister.zero = (res == 0) ? 1 : 0;
            sys.cpu.statusRegister.overflow = utils::sub_overflow(dst, src) ? 1 : 0;

            return sys;
        }

        inline momiji::System bra(momiji::System sys, const momiji::Instruction& instr)
        {
            sys.cpu.programCounter.value = instr.operands[0].value;
            return sys;
        }

        inline momiji::System ble(momiji::System sys, const momiji::Instruction& instr)
        {
            auto& statReg = sys.cpu.statusRegister;
            if ((statReg.negative == 0 && statReg.overflow == 1) ||
                (statReg.negative == 1 && statReg.overflow == 0) ||
                (statReg.zero == 1))
            {
                sys.cpu.programCounter.value = instr.operands[0].value;
            }
            else
            {
                ++sys.cpu.programCounter.value;
            }

            return sys;
        }

        inline momiji::System blt(momiji::System sys, const momiji::Instruction& instr)
        {
            auto& statReg = sys.cpu.statusRegister;

            if ((statReg.negative == 0 && statReg.overflow == 1) ||
                (statReg.negative == 1 && statReg.overflow == 0))
            {
                sys.cpu.programCounter.value = instr.operands[0].value;
            }
            else
            {
                ++sys.cpu.programCounter.value;
            }

            return sys;
        }

        inline momiji::System bge(momiji::System sys, const momiji::Instruction& instr)
        {
            auto& statReg = sys.cpu.statusRegister;

            if ((statReg.negative == 0 && statReg.overflow == 0) ||
                (statReg.negative == 1 && statReg.overflow == 1))
            {
                sys.cpu.programCounter.value = instr.operands[0].value;
            }
            else
            {
                ++sys.cpu.programCounter.value;
            }

            return sys;
        }

        inline momiji::System bgt(momiji::System sys, const momiji::Instruction& instr)
        {
            auto& statReg = sys.cpu.statusRegister;

            bool first_check = statReg.zero == 0 &&
                               statReg.negative == 0 &&
                               statReg.overflow == 0;

            bool second_check = statReg.zero == 0 &&
                                statReg.negative == 1 &&
                                statReg.overflow == 1;

            if (first_check || second_check)
            {
                sys.cpu.programCounter.value = instr.operands[0].value;
            }
            else
            {
                ++sys.cpu.programCounter.value;
            }

            return sys;
        }

        inline momiji::System beq(momiji::System sys, const momiji::Instruction& instr)
        {
            auto& statReg = sys.cpu.statusRegister;

            if (statReg.zero == 1)
            {
                sys.cpu.programCounter.value = instr.operands[0].value;
            }
            else
            {
                ++sys.cpu.programCounter.value;
            }

            return sys;
        }

        inline momiji::System bne(momiji::System sys, const momiji::Instruction& instr)
        {
            auto& statReg = sys.cpu.statusRegister;

            if (statReg.zero == 0)
            {
                sys.cpu.programCounter.value = instr.operands[0].value;
            }
            else
            {
                ++sys.cpu.programCounter.value;
            }

            return sys;
        }


        inline momiji::System jmp(momiji::System sys, const momiji::Instruction& instr)
        {
            return sys;
        }

        constexpr std::array<momiji::instr_fn_t, 3> move{ move8, move16, move32 };
        constexpr std::array<momiji::instr_fn_t, 3> add{ add8, add16, add32 };
        constexpr std::array<momiji::instr_fn_t, 3> sub{ sub8, sub16, sub32 };
        constexpr std::array<momiji::instr_fn_t, 3> cmp{ cmp8, cmp16, cmp32 };
    }
}
