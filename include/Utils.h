#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <type_traits>

namespace momiji
{
    namespace utils
    {
        // From Chaiscript
        // https://github.com/ChaiScript/ChaiScript/blob/develop/include/chaiscript/utility/hash.hpp
        template <typename Itr>
        static constexpr std::uint32_t hash(Itr begin, Itr end) noexcept
        {
            std::uint32_t hash = 0x811c9dc5;

            while (begin != end)
            {
                hash = (hash ^ *begin) * 0x1000193;
                ++begin;
            }

            return hash;
        }

        template <int N>
        static constexpr std::uint32_t hash(const char (&str)[N])
        {
            return hash(str, str + N - 1);
        }

        template <typename T>
        static constexpr std::uint32_t hash(const T& t)
        {
            return hash(t.begin(), t.end());
        }

        template <typename Enum>
        constexpr auto to_val(Enum e)
        {
            return static_cast<std::underlying_type_t<Enum>>(e);
        }
    }
}
