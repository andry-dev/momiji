#pragma once

#include <Utils.h>

#include <Decoder.h>
#include <Memory.h>
#include <System.h>

namespace momiji
{
    namespace utils
    {
        inline int isImmediate(const InstructionData& instr)
        {
            if (instr.op1 == OperandType::Immediate &&
                instr.mod1 == SpecialAddressingMode::Immediate)
            {
                return instr.size * 2;
            }

            if (instr.op1 == OperandType::Immediate &&
                instr.mod1 == SpecialAddressingMode::AbsoluteShort)
            {
                return 2;
            }

            if (instr.op1 == OperandType::Immediate &&
                instr.mod1 == SpecialAddressingMode::AbsoluteLong)
            {
                return 4;
            }

            return 0;
        }

        inline std::int32_t readImmediateFromPC(ExecutableMemoryView base,
                                                std::uint32_t pc,
                                                std::int16_t size)
        {
            switch (size)
            {
            case 1:
                return base.read8(pc + 3);

            case 2:
                return base.read16(pc + 2);

            case 4:
                return base.read32(pc + 2);
            }

            return 0;
        }

        inline std::int32_t readFromMemory(ExecutableMemoryView base,
                                           std::uint32_t offset,
                                           std::int16_t size)
        {
            switch (size)
            {
            case 1:
            {
                const std::uint32_t correct_offset =
                    utils::sign_extend<std::int8_t>(offset);
                return base.read8(correct_offset);
            }
            break;

            case 2:
            {
                const std::uint32_t correct_offset =
                    utils::sign_extend<std::int16_t>(offset);
                return base.read16(correct_offset);
            }
            break;

            case 4:
                return base.read32(offset);
                break;
            }

            return 0;
        }

        inline std::int32_t readOperand1(momiji::System& sys,
                                         const InstructionData& instr)
        {
            const auto regnum = utils::to_val(instr.mod1);
            const auto pc     = sys.cpu.programCounter.address;
            std::int32_t val  = 0;

            switch (instr.op1)
            {
            // d*
            case OperandType::DataRegister:
                val = sys.cpu.dataRegisters[regnum].value;
                break;

            // a*
            case OperandType::AddressRegister:
                val = sys.cpu.addressRegisters[regnum].value;
                break;

            // (a*)
            case OperandType::Address:
                val = sys.cpu.addressRegisters[regnum].value;
                val = readFromMemory(sys.mem, val, instr.size);
                break;

            // -(a*)
            case OperandType::AddressPre:
                sys.cpu.addressRegisters[regnum].value -= instr.size;
                val = sys.cpu.addressRegisters[regnum].value;
                val = readFromMemory(sys.mem, val, instr.size);
                break;

            // (a*)+
            case OperandType::AddressPost:
                val = sys.cpu.addressRegisters[regnum].value;
                val = readFromMemory(sys.mem, val, instr.size);
                sys.cpu.addressRegisters[regnum].value += instr.size;
                break;

            // index(a*)
            case OperandType::AddressOffset:
                val = readImmediateFromPC(sys.mem, pc, 2);
                val += sys.cpu.addressRegisters[regnum].value;
                val = readFromMemory(sys.mem, val, instr.size);
                break;

            // (index, a*, d*)
            case OperandType::AddressIndex:
            {
                val = sys.cpu.addressRegisters[regnum].value;

                const std::int16_t tmp   = readImmediateFromPC(sys.mem, pc, 2);
                const std::int8_t newreg = (tmp & 0xF000) >> 12;
                const std::int8_t index  = (tmp & 0x00FF);

                if (newreg < 8) // Data register [0, 7]
                {
                    val += sys.cpu.dataRegisters[newreg].value;
                }
                else // Address register [8, 15]
                {
                    val += sys.cpu.addressRegisters[newreg - 8].value;
                }

                val += index;

                val = readFromMemory(sys.mem, val, instr.size);
            }
            break;

            case OperandType::Immediate:
                switch (instr.mod1)
                {
                case SpecialAddressingMode::Immediate:
                    val = readImmediateFromPC(sys.mem, pc, instr.size);
                    break;

                case SpecialAddressingMode::AbsoluteShort:
                {
                    std::uint32_t addr = readImmediateFromPC(sys.mem, pc, 2);
                    addr               = utils::sign_extend<std::int16_t>(addr);
                    val = readFromMemory(sys.mem, addr, instr.size);
                }
                break;

                case SpecialAddressingMode::AbsoluteLong:
                    val = readImmediateFromPC(sys.mem, pc, 4);
                    val = readFromMemory(sys.mem, val, instr.size);
                    break;

                case SpecialAddressingMode::ProgramCounterIndex:
                    break;

                case SpecialAddressingMode::ProgramCounterOffset:
                    break;
                }
            }

            return val;
        }

        inline std::int8_t* readDestOp8(momiji::System& sys,
                                        const InstructionData& instr)
        {
            const auto regnum = utils::to_val(instr.mod2);
            const auto pc     = sys.cpu.programCounter.address;
            std::int32_t tmp  = 0;
            std::int8_t* val  = nullptr;

            switch (instr.op2)
            {
            // d*
            case OperandType::DataRegister:
                val = reinterpret_cast<std::int8_t*>(
                    &sys.cpu.dataRegisters[regnum].value);
                break;

            // a*
            case OperandType::AddressRegister:
                val = reinterpret_cast<std::int8_t*>(
                    &sys.cpu.addressRegisters[regnum].value);
                break;

            // (a*)
            case OperandType::Address:
                tmp = sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int8_t*>(sys.mem.data() + tmp);
                break;

            // -(a*)
            case OperandType::AddressPre:
                sys.cpu.addressRegisters[regnum].value -= instr.size;
                tmp = sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int8_t*>(sys.mem.data() + tmp);
                break;

            // (a*)+
            case OperandType::AddressPost:
                tmp = sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int8_t*>(sys.mem.data() + tmp);
                sys.cpu.addressRegisters[regnum].value += instr.size;
                break;

            // index(a*)
            case OperandType::AddressOffset:
                tmp = readImmediateFromPC(sys.mem, pc + isImmediate(instr), 2);
                tmp += sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int8_t*>(sys.mem.data() + tmp);
                break;

            // (index, a*, d*)
            case OperandType::AddressIndex:
            {
                tmp = readImmediateFromPC(sys.mem, pc + isImmediate(instr), 2);

                const std::uint8_t newreg = (tmp & 0xF000) >> 12;
                const std::uint8_t index  = (tmp & 0x00FF);

                tmp = sys.cpu.addressRegisters[regnum].value;

                if (newreg < 8) // Data register [0, 7]
                {
                    tmp += sys.cpu.dataRegisters[newreg].value;
                }
                else // Address register [8, 15]
                {
                    tmp += sys.cpu.addressRegisters[newreg - 8].value;
                }

                tmp += index;

                val = reinterpret_cast<std::int8_t*>(sys.mem.data() + tmp);
            }
            break;

            case OperandType::Immediate:
                switch (instr.mod2)
                {
                case SpecialAddressingMode::AbsoluteShort:
                {
                    tmp = readImmediateFromPC(
                        sys.mem, pc + isImmediate(instr), 2);

                    tmp = utils::sign_extend<std::int16_t>(tmp);
                    val = reinterpret_cast<std::int8_t*>(sys.mem.data() + tmp);
                }
                break;

                case SpecialAddressingMode::AbsoluteLong:
                {
                    tmp = readImmediateFromPC(
                        sys.mem, pc + isImmediate(instr), 4);

                    val = reinterpret_cast<std::int8_t*>(sys.mem.data() + tmp);
                }
                break;

                case SpecialAddressingMode::ProgramCounterIndex:
                    break;

                case SpecialAddressingMode::ProgramCounterOffset:
                    break;
                }
            }

            return val;
        }

        inline std::int16_t* readDestOp16(momiji::System& sys,
                                          const InstructionData& instr)
        {
            const auto regnum = utils::to_val(instr.mod2);
            const auto pc     = sys.cpu.programCounter.address;
            std::int32_t tmp  = 0;
            std::int16_t* val = nullptr;

            switch (instr.op2)
            {
            // d*
            case OperandType::DataRegister:
                val = reinterpret_cast<std::int16_t*>(
                    &sys.cpu.dataRegisters[regnum].value);
                break;

            // a*
            case OperandType::AddressRegister:
                val = reinterpret_cast<std::int16_t*>(
                    &sys.cpu.addressRegisters[regnum].value);
                break;

            // (a*)
            case OperandType::Address:
                tmp = sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int16_t*>(sys.mem.data() + tmp);
                break;

            // -(a*)
            case OperandType::AddressPre:
                sys.cpu.addressRegisters[regnum].value -= instr.size;
                tmp = sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int16_t*>(sys.mem.data() + tmp);
                break;

            // (a*)+
            case OperandType::AddressPost:
                tmp = sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int16_t*>(sys.mem.data() + tmp);
                sys.cpu.addressRegisters[regnum].value += instr.size;
                break;

            // index(a*)
            case OperandType::AddressOffset:
                tmp = readImmediateFromPC(sys.mem, pc + isImmediate(instr), 2);
                tmp += sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int16_t*>(sys.mem.data() + tmp);
                break;

            // (index, a*, d*)
            case OperandType::AddressIndex:
            {
                tmp = readImmediateFromPC(sys.mem, pc + isImmediate(instr), 2);

                const std::uint8_t newreg = (tmp & 0xF000) >> 12;
                const std::uint8_t index  = (tmp & 0x00FF);

                tmp = sys.cpu.addressRegisters[regnum].value;

                if (newreg < 8) // Data register [0, 7]
                {
                    tmp += sys.cpu.dataRegisters[newreg].value;
                }
                else // Address register [8, 15]
                {
                    tmp += sys.cpu.addressRegisters[newreg - 8].value;
                }

                tmp += index;

                val = reinterpret_cast<std::int16_t*>(sys.mem.data() + tmp);
            }
            break;

            case OperandType::Immediate:
                switch (instr.mod2)
                {
                case SpecialAddressingMode::AbsoluteShort:
                {
                    std::uint32_t addr = readImmediateFromPC(
                        sys.mem, pc + isImmediate(instr), 2);

                    addr = utils::sign_extend<std::int16_t>(addr);
                    val =
                        reinterpret_cast<std::int16_t*>(sys.mem.data() + addr);
                }
                break;

                case SpecialAddressingMode::AbsoluteLong:
                {
                    std::uint32_t addr = readImmediateFromPC(
                        sys.mem, pc + isImmediate(instr), 4);

                    val =
                        reinterpret_cast<std::int16_t*>(sys.mem.data() + addr);
                }
                break;

                case SpecialAddressingMode::ProgramCounterIndex:
                    break;

                case SpecialAddressingMode::ProgramCounterOffset:
                    break;
                }
            }

            return val;
        }

        inline std::int32_t* readDestOp32(momiji::System& sys,
                                          const InstructionData& instr)
        {
            const auto regnum = utils::to_val(instr.mod2);
            const auto pc     = sys.cpu.programCounter.address;
            std::int32_t tmp  = 0;
            std::int32_t* val = nullptr;

            switch (instr.op2)
            {
            // d*
            case OperandType::DataRegister:
                val = &sys.cpu.dataRegisters[regnum].value;
                break;

            // a*
            case OperandType::AddressRegister:
                val = &sys.cpu.addressRegisters[regnum].value;
                break;

            // (a*)
            case OperandType::Address:
                tmp = sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int32_t*>(sys.mem.data() + tmp);
                break;

            // -(a*)
            case OperandType::AddressPre:
                sys.cpu.addressRegisters[regnum].value -= instr.size;
                tmp = sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int32_t*>(sys.mem.data() + tmp);
                break;

            // (a*)+
            case OperandType::AddressPost:
                tmp = sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int32_t*>(sys.mem.data() + tmp);
                sys.cpu.addressRegisters[regnum].value += instr.size;
                break;

            // index(a*)
            case OperandType::AddressOffset:
                tmp = readImmediateFromPC(sys.mem, pc + isImmediate(instr), 2);

                tmp += sys.cpu.addressRegisters[regnum].value;
                val = reinterpret_cast<std::int32_t*>(sys.mem.data() + tmp);
                break;

            // (index, a*, d*)
            case OperandType::AddressIndex:
            {
                std::uint16_t tmp =
                    readImmediateFromPC(sys.mem, pc + isImmediate(instr), 2);

                const std::uint8_t newreg = (tmp & 0xF000) >> 12;
                const std::uint8_t index  = (tmp & 0x00FF);

                tmp = sys.cpu.addressRegisters[regnum].value;

                if (newreg < 8) // Data register [0, 7]
                {
                    tmp += sys.cpu.dataRegisters[newreg].value;
                }
                else // Address register [8, 15]
                {
                    tmp += sys.cpu.addressRegisters[newreg - 8].value;
                }

                tmp += index;

                val = reinterpret_cast<std::int32_t*>(sys.mem.data() + tmp);
            }
            break;

            case OperandType::Immediate:
                switch (instr.mod2)
                {
                case SpecialAddressingMode::AbsoluteShort:
                {
                    std::uint32_t addr = readImmediateFromPC(
                        sys.mem, pc + isImmediate(instr), 2);

                    addr = utils::sign_extend<std::int16_t>(addr);
                    val =
                        reinterpret_cast<std::int32_t*>(sys.mem.data() + addr);
                }
                break;

                case SpecialAddressingMode::AbsoluteLong:
                {
                    std::uint32_t addr = readImmediateFromPC(
                        sys.mem, pc + isImmediate(instr), 4);
                    val =
                        reinterpret_cast<std::int32_t*>(sys.mem.data() + addr);
                }
                break;

                case SpecialAddressingMode::ProgramCounterIndex:
                    break;

                case SpecialAddressingMode::ProgramCounterOffset:
                    break;
                }
            }

            return val;
        }
    } // namespace utils
} // namespace momiji
