#pragma once

#include "System.h"
#include "Parser.h"

#include <iostream>

namespace momiji
{
    namespace op_impl
    {
        inline momiji::system move8(momiji::system sys, const momiji::instruction& instr)
        {
            union
            {
                std::int8_t arr[4];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate: {
                pun.val = instr.operands[0].value & 0x000000FF;
            } break;

            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | (pun.arr[0] & 0x000000FF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | (pun.arr[0] & 0x000000FF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system move16(momiji::system sys, const momiji::instruction& instr)
        {
            union
            {
                std::int16_t arr[2];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;

            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | (pun.arr[0] & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | (pun.arr[0] & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }



            return sys;

        }

        inline momiji::system move32(momiji::system sys, const momiji::instruction& instr)
        {
            std::int32_t tmp = 0;
            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                tmp = instr.operands[0].value;
                break;

            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    tmp = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    tmp = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
            }

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system moveq(momiji::system sys, const momiji::instruction& instr)
        {
            std::int8_t trunc = instr.operands[0].value & 0x000000FF;
            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | trunc;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;

                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | trunc;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system add8(momiji::system sys, const momiji::instruction& instr)
        {
            union
            {
                std::int8_t arr[4];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;
                break;

            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp + pun.arr[0]) & 0x000000FF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp + pun.arr[0]) & 0x000000FF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system add16(momiji::system sys, const momiji::instruction& instr)
        {
            union
            {
                std::int16_t arr[2];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;
                break;

            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp + pun.arr[0]) & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp + pun.arr[0]) & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system add32(momiji::system sys, const momiji::instruction& instr)
        {
            std::int32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                val = instr.operands[0].value;
                break;

            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = tmp + val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = tmp + val;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system sub8(momiji::system sys, const momiji::instruction& instr)
        {
            union
            {
                std::int8_t arr[4];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;
                break;

            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp - pun.arr[0]) & 0x000000FF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp - pun.arr[0]) & 0x000000FF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system sub16(momiji::system sys, const momiji::instruction& instr)
        {
            union
            {
                std::int16_t arr[2];
                std::int32_t val;
            } pun;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                pun.val = instr.operands[0].value & 0x0000FFFF;
                break;

            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    pun.val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp - pun.arr[0]) & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp - pun.arr[0]) & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system sub32(momiji::system sys, const momiji::instruction& instr)
        {
            std::int32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                val = instr.operands[0].value;
                break;

            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp = tmp - val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp = tmp - val;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system muls(momiji::system sys, const momiji::instruction& instr)
        {
            std::int32_t val = 0;

            bool signedness = false;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                val = instr.operands[0].value;
                break;
            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;
                case register_type::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;
                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                tmp *= val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                tmp *= val;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system mulu(momiji::system sys, const momiji::instruction& instr)
        {
            std::uint32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                val = static_cast<std::uint32_t>(
                        instr.operands[0].value & 0x0000FFFF);
                break;
            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    val = static_cast<std::uint32_t>(
                            sys.cpu.addressRegisters[instr.operands[0].value].value);
                    break;
                case register_type::Data:
                    val = static_cast<std::uint32_t>(
                            sys.cpu.dataRegisters[instr.operands[0].value].value);
                    break;
                case register_type::Special:
                    break;
                }
                break;
            }

            std::uint32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value & 0x0000FFFF;
                tmp = tmp * val;
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value & 0x0000FFFF;
                tmp = tmp * val;
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system divs(momiji::system sys, const momiji::instruction& instr)
        {
            std::int32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                val = instr.operands[0].value;
                break;
            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;
                case register_type::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;
                case register_type::Special:
                    break;
                }
                break;
            }

            std::int32_t tmp = 0;
            std::int32_t quotient = 0;
            std::int32_t reminder = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system divu(momiji::system sys, const momiji::instruction& instr)
        {
            std::uint32_t val = 0;

            switch (instr.operands[0].operandType)
            {
            case operand_type::Immediate:
                val = instr.operands[0].value;
                break;
            case operand_type::Register:
                switch (instr.operands[0].registerType)
                {
                case register_type::Address:
                    val = sys.cpu.addressRegisters[instr.operands[0].value].value;
                    break;
                case register_type::Data:
                    val = sys.cpu.dataRegisters[instr.operands[0].value].value;
                    break;
                case register_type::Special:
                    break;
                }
                break;
            }

            std::uint32_t tmp = 0;
            std::uint32_t quotient = 0;
            std::uint32_t reminder = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = sys.cpu.addressRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = sys.cpu.dataRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                sys.cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return sys;
        }

        inline momiji::system swap(momiji::system sys, const momiji::instruction& instr)
        {
            return sys;
        }

        inline momiji::system exg(momiji::system sys, const momiji::instruction& instr)
        {
            return sys;
        }

        inline momiji::system jmp(momiji::system sys, const momiji::instruction& instr)
        {
            return sys;
        }
    }
}
