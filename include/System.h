#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace momiji
{
    template <typename IntType>
    struct register_t
    {
        IntType value;

        using int_type = IntType;
    };

    using data_register_t = register_t<std::int32_t>;
    using address_register_t = register_t<std::int32_t>;
    using program_counter_t = register_t<std::int32_t>;
    using status_register_t = register_t<std::int16_t>;

    struct label
    {
        label(std::int32_t name_hash, std::int32_t idx)
            : name_hash(name_hash), idx(idx)
        {
        }
        
        label() = default;
        ~label() = default;
        label(const label&) = default;
        label& operator=(const label&) = default;
        label(label&&) = default;
        label& operator=(label&&) = default;

        std::int32_t name_hash = 0;
        std::int32_t idx = 0;
    };

    struct cpu_t
    {
        cpu_t()
            : dataRegisters({0})
            , addressRegisters({0})
        {

        }

        std::array<data_register_t, 8> dataRegisters;
        std::array<address_register_t, 8> addressRegisters;

        status_register_t statusRegister;
        program_counter_t programCounter;
    };

    struct system
    {
        cpu_t cpu;
        std::vector<momiji::label> labels;
    };
}
