#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <Parser.h>

namespace momiji
{
    template <typename IntType>
    struct Register
    {
        IntType value;

        using value_type = IntType;
    };

    using DataRegister = Register<std::int32_t>;
    using AddressRegister = Register<std::int32_t>;
    using ProgramCounter = Register<std::int32_t>;

    struct StatusRegister
    {
        StatusRegister()
            : extend(0), negative(0), zero(0), overflow(0), carry(0)
        { }

        std::uint8_t extend : 1;
        std::uint8_t negative : 1;
        std::uint8_t zero : 1;
        std::uint8_t overflow : 1;
        std::uint8_t carry : 1;
    };

    struct Label
    {
        Label(std::int32_t name_hash, std::int32_t idx)
            : name_hash(name_hash), idx(idx)
        { }

        Label() = default;
        ~Label() = default;
        Label(const Label&) = default;
        Label& operator=(const Label&) = default;
        Label(Label&&) = default;
        Label& operator=(Label&&) = default;

        std::int32_t name_hash = 0;
        std::int32_t idx = 0;
    };

    struct Cpu
    {
        Cpu()
            : dataRegisters({0})
            , addressRegisters({0})
        {

        }

        std::array<DataRegister, 8> dataRegisters;
        std::array<AddressRegister, 8> addressRegisters;

        StatusRegister statusRegister;
        ProgramCounter programCounter;
    };

    struct System
    {
        Cpu cpu;
        std::vector<momiji::Instruction> instructions;
    };
}
