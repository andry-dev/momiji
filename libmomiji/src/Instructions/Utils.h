#include <asl/types>
#pragma once

#include <Utils.h>

#include <Decoder.h>
#include <Memory.h>
#include <System.h>

#include <asl/detect_features>

namespace momiji::utils
{
#ifdef ASL_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif

    inline std::int32_t readImmediateFromPC(ExecutableMemoryView base,
                                            std::uint32_t pc,
                                            std::int16_t size)
    {
        switch (size)
        {
        case 1: {
            const auto val = base.read8(pc + 2);
            return std::int32_t(val.value_or(0));
        }

        case 2: {
            const auto val = base.read16(pc + 2);
            return std::int32_t(val.value_or(0));
        }

        case 4: {
            const auto val = base.read32(pc + 2);
            return std::int32_t(val.value_or(0));
        }
        }

        return 0;
    }

    inline std::int32_t readFromMemory(ExecutableMemoryView base,
                                       std::int32_t offset,
                                       std::int16_t size)
    {
        switch (size)
        {
        case 1: {
            const auto correct_offset = utils::sign_extend<std::int8_t>(offset);
            const auto val            = base.read8(correct_offset);
            return std::int32_t(val.value_or(0));
        }

        case 2: {
            const auto correct_offset =
                utils::sign_extend<std::int16_t>(offset);
            const auto val = base.read16(correct_offset);
            return std::int32_t(val.value_or(0));
        }

        case 4: {
            const auto val = base.read32(offset);
            return std::int32_t(val.value_or(0));
        }
        }

        return 0;
    }

    inline std::int32_t readOperandVal(momiji::System& sys,
                                       const InstructionData& instr,
                                       std::int8_t op)
    {
        const auto regnum =
            utils::to_val(asl::saccess(instr.addressingMode, op));
        const auto pc    = sys.cpu.programCounter.address;
        std::int32_t val = 0;

        switch (asl::saccess(instr.operandType, op))
        {
        // d*
        case OperandType::DataRegister:
            val = asl::saccess(sys.cpu.dataRegisters, regnum).value;
            break;

        // a*
        case OperandType::AddressRegister:
            val = asl::saccess(sys.cpu.addressRegisters, regnum).value;
            break;

        // (a*)
        case OperandType::Address:
            val = asl::saccess(sys.cpu.addressRegisters, regnum).value;
            val = readFromMemory(sys.mem, val, instr.size);
            break;

        // -(a*)
        case OperandType::AddressPre:
            sys.cpu.addressRegisters[regnum].value -= instr.size;
            val = asl::saccess(sys.cpu.addressRegisters, regnum).value;
            val = readFromMemory(sys.mem, val, instr.size);
            break;

        // (a*)+
        case OperandType::AddressPost:
            val = asl::saccess(sys.cpu.addressRegisters, regnum).value;
            val = readFromMemory(sys.mem, val, instr.size);
            sys.cpu.addressRegisters[regnum].value += instr.size;
            break;

        // index(a*)
        case OperandType::AddressOffset:
            val = readImmediateFromPC(sys.mem, pc, 2);
            val += asl::saccess(sys.cpu.addressRegisters, regnum).value;

            val = readFromMemory(sys.mem, val, instr.size);
            break;

        // (index, a*, d*)
        case OperandType::AddressIndex: {
            val = asl::saccess(sys.cpu.addressRegisters, regnum).value;

            const std::int32_t tmp = readImmediateFromPC(sys.mem, pc, 2);

            const auto newreg = std::int8_t((tmp & 0xF000) >> 12);
            const auto index  = std::int8_t((tmp & 0x00FF));

            if (newreg < 8) // Data register [0, 7]
            {
                val += asl::saccess(sys.cpu.dataRegisters, newreg).value;
            }
            else // Address register [8, 15]
            {
                val += asl::saccess(sys.cpu.addressRegisters, newreg - 8).value;
            }

            val += index;

            val = readFromMemory(sys.mem, val, instr.size);
        }
        break;

        case OperandType::Immediate:
            switch (asl::saccess(instr.addressingMode, op))
            {
            case SpecialAddressingMode::Immediate:
                val = readImmediateFromPC(sys.mem, pc, instr.size);
                break;

            case SpecialAddressingMode::AbsoluteShort: {
                std::int32_t addr = readImmediateFromPC(sys.mem, pc, 2);

                addr = utils::sign_extend<std::int16_t>(addr);

                val = readFromMemory(sys.mem, addr, instr.size);
            }
            break;

            case SpecialAddressingMode::AbsoluteLong:
                val = readImmediateFromPC(sys.mem, pc, 4);
                val = readFromMemory(sys.mem, val, instr.size);
                break;

            case SpecialAddressingMode::ProgramCounterIndex:
            case SpecialAddressingMode::ProgramCounterOffset:
                break;
            }
            break;
        }

        return val;
    }

    template <typename To>
    inline To* readOperandPtr(momiji::System& sys,
                              const InstructionData& instr,
                              std::int8_t op)
    {
        const auto regnum =
            utils::to_val(asl::saccess(instr.addressingMode, op));

        const auto pc    = sys.cpu.programCounter.address;
        std::int32_t tmp = 0;
        To* val          = nullptr;

        switch (asl::saccess(instr.operandType, op))
        {
        // d*
        case OperandType::DataRegister:
            val = reinterpret_cast<To*>(
                &asl::saccess(sys.cpu.dataRegisters, regnum).value);
            break;

        // a*
        case OperandType::AddressRegister:
            val = reinterpret_cast<To*>(
                &asl::saccess(sys.cpu.addressRegisters, regnum).value);
            break;

        // (a*)
        case OperandType::Address:
            tmp = asl::saccess(sys.cpu.addressRegisters, regnum).value;
            val = reinterpret_cast<To*>(sys.mem.data() + tmp);
            break;

        // -(a*)
        case OperandType::AddressPre:
            asl::saccess(sys.cpu.addressRegisters, regnum).value -= instr.size;
            tmp = asl::saccess(sys.cpu.addressRegisters, regnum).value;
            val = reinterpret_cast<To*>(sys.mem.data() + tmp);
            break;

        // (a*)+
        case OperandType::AddressPost:
            tmp = asl::saccess(sys.cpu.addressRegisters, regnum).value;
            val = reinterpret_cast<To*>(sys.mem.data() + tmp);
            asl::saccess(sys.cpu.addressRegisters, regnum).value += instr.size;
            break;

        // offset(a*)
        case OperandType::AddressOffset:
            tmp = readImmediateFromPC(
                sys.mem,
                pc + std::uint32_t(resolveOp1Size(instr, op)),
                sizeof(To));

            tmp += asl::saccess(sys.cpu.addressRegisters, regnum).value;
            val = reinterpret_cast<To*>(sys.mem.data() + tmp);
            break;

        // (offset, a*, **)
        case OperandType::AddressIndex: {
            tmp = readImmediateFromPC(
                sys.mem,
                pc + std::uint32_t(resolveOp1Size(instr, op)),
                sizeof(To));

            const auto newreg = std::int8_t((tmp & 0xF000) >> 12);
            const auto index  = std::int8_t(tmp & 0x00FF);

            tmp = asl::saccess(sys.cpu.addressRegisters, regnum).value;

            if (newreg < 8) // Data register [0, 7]
            {
                tmp += asl::saccess(sys.cpu.dataRegisters, newreg).value;
            }
            else // Address register [8, 15]
            {
                tmp += asl::saccess(sys.cpu.addressRegisters, newreg - 8).value;
            }

            tmp += index;

            val = reinterpret_cast<To*>(sys.mem.data() + tmp);
        }
        break;

        case OperandType::Immediate:
            switch (asl::saccess(instr.addressingMode, op))
            {
            case SpecialAddressingMode::AbsoluteShort: {
                tmp = readImmediateFromPC(
                    sys.mem, pc + std::uint32_t(resolveOp1Size(instr, op)), 2);

                tmp = utils::sign_extend<std::int16_t>(tmp);
                val = reinterpret_cast<To*>(sys.mem.data() + tmp);
            }
            break;

            case SpecialAddressingMode::AbsoluteLong: {
                tmp = readImmediateFromPC(
                    sys.mem, pc + std::uint32_t(resolveOp1Size(instr, op)), 4);

                val = reinterpret_cast<To*>(sys.mem.data() + tmp);
            }
            break;

            case SpecialAddressingMode::ProgramCounterIndex:
                break;

            case SpecialAddressingMode::ProgramCounterOffset:
                break;

            case SpecialAddressingMode::Immediate:
                val = reinterpret_cast<To*>(sys.mem.data() + pc);
                break;
            }
            break;
        }

        return val;
    }

#ifdef ASL_CLANG
#pragma clang diagnostic pop
#endif
} // namespace momiji::utils
