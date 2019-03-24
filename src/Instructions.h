#pragma once

#include "CPU.h"
#include "Parser.h"

#include <iostream>

namespace momiji
{
    namespace op_impl
    {
        inline cpu_t move8(cpu_t cpu, const momiji::instruction& instr)
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
                    pun.val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | (pun.arr[0] & 0x000000FF);
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | (pun.arr[0] & 0x000000FF);
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t move16(cpu_t cpu, const momiji::instruction& instr)
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
                    pun.val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | (pun.arr[0] & 0x0000FFFF);
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | (pun.arr[0] & 0x0000FFFF);
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }



            return cpu;

        }

        inline cpu_t move32(cpu_t cpu, const momiji::instruction& instr)
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
                    tmp = cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    tmp = cpu.dataRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Special:
                    break;
                }
            }

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t moveq(cpu_t cpu, const momiji::instruction& instr)
        {
            std::int8_t trunc = instr.operands[0].value & 0x000000FF;
            std::int32_t tmp = 0;

            switch (instr.operands[1].registerType)
            {
            case register_type::Address:
                tmp = cpu.addressRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | trunc;
                cpu.addressRegisters[instr.operands[1].value].value = tmp;

                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;

                tmp = (tmp & 0xFFFFFF00) | trunc;
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t add8(cpu_t cpu, const momiji::instruction& instr)
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
                    pun.val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp + pun.arr[0]) & 0x000000FF);
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp + pun.arr[0]) & 0x000000FF);
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t add16(cpu_t cpu, const momiji::instruction& instr)
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
                    pun.val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp + pun.arr[0]) & 0x0000FFFF);
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp + pun.arr[0]) & 0x0000FFFF);
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t add32(cpu_t cpu, const momiji::instruction& instr)
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
                    val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                tmp = tmp + val;
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                tmp = tmp + val;
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t sub8(cpu_t cpu, const momiji::instruction& instr)
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
                    pun.val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp - pun.arr[0]) & 0x000000FF);
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFFFF00) | ((tmp - pun.arr[0]) & 0x000000FF);
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t sub16(cpu_t cpu, const momiji::instruction& instr)
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
                    pun.val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    pun.val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp - pun.arr[0]) & 0x0000FFFF);
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                tmp = (tmp & 0xFFFF0000) | ((tmp - pun.arr[0]) & 0x0000FFFF);
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t sub32(cpu_t cpu, const momiji::instruction& instr)
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
                    val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;

                case register_type::Data:
                    val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                tmp = tmp - val;
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                tmp = tmp - val;
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t muls(cpu_t cpu, const momiji::instruction& instr)
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
                    val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;
                case register_type::Data:
                    val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                tmp *= val;
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                tmp *= val;
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t mulu(cpu_t cpu, const momiji::instruction& instr)
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
                            cpu.addressRegisters[instr.operands[0].value].value);
                    break;
                case register_type::Data:
                    val = static_cast<std::uint32_t>(
                            cpu.dataRegisters[instr.operands[0].value].value);
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value & 0x0000FFFF;
                tmp = tmp * val;
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value & 0x0000FFFF;
                tmp = tmp * val;
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t divs(cpu_t cpu, const momiji::instruction& instr)
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
                    val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;
                case register_type::Data:
                    val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t divu(cpu_t cpu, const momiji::instruction& instr)
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
                    val = cpu.addressRegisters[instr.operands[0].value].value;
                    break;
                case register_type::Data:
                    val = cpu.dataRegisters[instr.operands[0].value].value;
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
                tmp = cpu.addressRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                cpu.addressRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Data:
                tmp = cpu.dataRegisters[instr.operands[1].value].value;
                quotient = tmp / val;
                reminder = tmp % val;
                tmp = ((quotient & 0x0000FFFF) << 16) | (reminder & 0x0000FFFF);
                cpu.dataRegisters[instr.operands[1].value].value = tmp;
                break;

            case register_type::Special:
                break;
            }

            return cpu;
        }

        inline cpu_t swap(cpu_t cpu, const momiji::instruction& instr)
        {
            return cpu;
        }

        inline cpu_t exg(cpu_t cpu, const momiji::instruction& instr)
        {
            return cpu;
        }
    }
}
