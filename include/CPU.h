#pragma once

#include <array>
#include <cstdint>

namespace momiji
{
    template <typename IntType>
    struct register_t
    {
        IntType value;
    };

    using data_register_t = register_t<std::int32_t>;

    using address_register_t = register_t<std::int32_t>;

    using program_counter_t = register_t<std::int32_t>;

    struct cpu_t
    {
        cpu_t()
            : dataRegisters({0})
            , addressRegisters({0})
        {

        }
        std::array<data_register_t, 8> dataRegisters;
        std::array<address_register_t, 8> addressRegisters;

        program_counter_t pc;
    };
}
