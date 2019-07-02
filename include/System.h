#pragma once

#include <Parser.h>
#include <array>
#include <cstdint>
#include <vector>

#include <Utils.h>
#include <gsl/span>

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

    struct StatusRegister
    {
        StatusRegister()
            : extend(0)
            , negative(0)
            , zero(0)
            , overflow(0)
            , carry(0)
        {
        }

        std::uint8_t extend : 1;   // E
        std::uint8_t negative : 1; // N
        std::uint8_t zero : 1;     // Z
        std::uint8_t overflow : 1; // V
        std::uint8_t carry : 1;    // C
    };

    struct ProgramCounter
    {
        std::uint32_t address;
    };

    struct Label
    {
        Label(std::int32_t name_hash, std::int32_t idx)
            : name_hash(name_hash)
            , idx(idx)
        {
        }

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
            : dataRegisters({ 0 })
            , addressRegisters({ 0 })
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
        MemoryType mem;
    };

    inline gsl::span<const std::uint16_t> make_memory_view(const System& sys)
    {
        return { sys.mem.data(), sys.mem.size() };
    }

    inline gsl::span<std::uint16_t> make_memory_view(System& sys)
    {
        return { sys.mem.data(), sys.mem.size() };
    }

} // namespace momiji
