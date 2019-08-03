#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>

#include <momiji/Decoder.h>

namespace momiji::utils
{
    namespace details
    {
        template <typename T>
        struct deleted_type : std::false_type
        {
        };
    } // namespace details

    // From Chaiscript
    // https://github.com/ChaiScript/ChaiScript/blob/develop/include/chaiscript/utility/hash.hpp
    template <typename Itr>
    static constexpr std::uint32_t hash(Itr begin, Itr end) noexcept
    {
        std::uint32_t hash = 0x811c9dc5;

        while (begin != end)
        {
            hash = (hash ^ std::uint32_t(*begin)) * 0x1000193;
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

    template <typename Enum, typename T>
    constexpr auto to_enum(T val)
    {
        return static_cast<Enum>(val);
    }

    template <typename T>
    constexpr bool add_overflow(T a, T b)
    {
        // a + b
        return (b > 0) && (a > (std::numeric_limits<T>::max() - b));
    }

    template <typename T>
    constexpr bool sub_overflow(T a, T b)
    {
        // a - b
        return (b < 0) && (a > (std::numeric_limits<T>::max() + b));
    }

    template <typename To>
    constexpr std::int32_t sign_extend(std::int32_t)
    {
        static_assert(details::deleted_type<To>::value,
                      "\nWrong type passed to sign_extend."
                      "\nYou can only pass std::int8_t or std::int16_t.");

        return 0;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"

    template <>
    constexpr std::int32_t sign_extend<std::int8_t>(std::int32_t val)
    {
        std::int32_t ret  = val & 0x000000FF;
        std::uint8_t sign = std::uint8_t(ret & 0x000000FF);

        if ((sign & 0b10000000) != 0)
        {
            ret = std::int32_t(ret | 0xFFFFFF00);
        }

        return ret;
    }

    template <>
    constexpr std::int32_t sign_extend<std::int16_t>(std::int32_t val)
    {
        std::int32_t ret  = val & 0x0000FFFF;
        std::uint8_t sign = std::uint8_t((ret & 0x0000FF00) >> 8);

        if ((sign & 0b10000000) != 0)
        {
            ret = std::int32_t(ret | 0xFFFF0000);
        }

        return ret;
    }

#pragma clang diagnostic pop

    constexpr std::int64_t make_kb(std::int64_t n)
    {
        return n * 1024;
    }

    constexpr std::int64_t make_mb(std::int64_t n)
    {
        return 1024 * make_kb(n);
    }

    constexpr std::int64_t make_gb(std::int64_t n)
    {
        return 1024 * make_mb(n);
    }

} // namespace momiji::utils
