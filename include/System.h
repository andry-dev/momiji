#pragma once

#include <array>
#include <cstdint>
#include <vector>

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
    using StatusRegister = Register<std::int16_t>;

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
        std::vector<momiji::Label> labels;
    };
}
