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
    using compare_result_t = register_t<std::int32_t>;

    struct label
    {
        std::int32_t name_hash;
        int idx = 0;
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

        compare_result_t comparereResult;
        program_counter_t programCounter;
    };

    struct system
    {
        cpu_t cpu;
        std::vector<momiji::label> labels;
    };
}
