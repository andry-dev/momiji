#pragma once

#include <array>
#include <cstdint>
#include <momiji/Parser.h>
#include <optional>
#include <vector>

#include <momiji/Memory.h>
#include <momiji/Utils.h>

namespace momiji
{
    namespace traps
    {
        struct InvalidMemoryRead
        {
            std::int32_t address;
        };

        struct InvalidMemoryWrite
        {
            std::int32_t address;
            std::int32_t value;
        };

        struct DivisionByZero
        {
        };

        struct IllegalInstruction
        {
        };
    } // namespace traps

    using TrapType = std::variant<traps::InvalidMemoryRead,
                                  traps::InvalidMemoryWrite,
                                  traps::DivisionByZero,
                                  traps::IllegalInstruction>;

    template <typename IntType, typename PhantomTag>
    struct Register
    {
        using value_type = IntType;

        Register()                = default;
        ~Register()               = default;
        Register(const Register&) = default;
        Register& operator=(const Register&) = default;
        Register(Register&&) noexcept        = default;
        Register& operator=(Register&&) noexcept = default;

        Register(IntType val)
            : value(val)
        {
        }

        [[nodiscard]] value_type raw() const
        {
            return value;
        }

        Register& operator=(IntType oth)
        {
            value = oth;
            return *this;
        }

        Register& operator++()
        {
            ++value;

            return *this;
        }

        Register operator++(int)
        {
            Register tmp { *this };

            operator++();

            return tmp;
        }

        Register& operator--()
        {
            --value;

            return *this;
        }

        Register operator--(int)
        {
            Register tmp { *this };

            operator--();

            return tmp;
        }

        Register& operator+=(Register rhs)
        {
            value += rhs.value;

            return *this;
        }

        template <typename U>
        Register& operator+=(U rhs)
        {
            value += rhs;

            return *this;
        }

        Register& operator-=(Register rhs)
        {
            value -= rhs.value;

            return *this;
        }

        template <typename U>
        Register& operator-=(U rhs)
        {
            value -= rhs;

            return *this;
        }

        Register& operator*=(Register rhs)
        {
            value *= rhs.value;

            return *this;
        }

        template <typename U>
        Register& operator*=(U rhs)
        {
            value *= rhs;

            return *this;
        }

        Register& operator&=(Register rhs)
        {
            value &= rhs.value;

            return *this;
        }

        template <typename U>
        Register& operator&=(U rhs)
        {
            value &= rhs;

            return *this;
        }

        Register& operator|=(Register rhs)
        {
            value |= rhs.value;

            return *this;
        }

        template <typename U>
        Register& operator|=(U rhs)
        {
            value |= rhs;

            return *this;
        }

        Register& operator^=(Register rhs)
        {
            value ^= rhs.value;

            return *this;
        }

        template <typename U>
        Register& operator^=(U rhs)
        {
            value ^= rhs;

            return *this;
        }

        Register& operator>>=(Register rhs)
        {
            value >>= rhs.value;

            return *this;
        }

        template <typename U>
        Register& operator>>=(U rhs)
        {
            value >>= rhs;

            return *this;
        }

        Register& operator<<=(Register rhs)
        {
            value <<= rhs.value;

            return *this;
        }

        template <typename U>
        Register& operator<<=(U rhs)
        {
            value <<= rhs;

            return *this;
        }

        friend Register operator+(Register lhs, Register rhs)
        {
            lhs.value += rhs.value;

            return lhs;
        }

        template <typename U>
        friend Register operator+(Register lhs, U rhs)
        {
            lhs.value += rhs;

            return lhs;
        }

        friend Register operator-(Register lhs, Register rhs)
        {
            lhs.value -= rhs.value;

            return lhs;
        }

        template <typename U>
        friend Register operator-(Register lhs, U rhs)
        {
            lhs.value -= rhs;

            return lhs;
        }

        friend Register operator*(Register lhs, Register rhs)
        {
            lhs.value *= rhs.value;

            return lhs;
        }

        template <typename U>
        friend Register operator*(Register lhs, U rhs)
        {
            lhs.value *= rhs;

            return lhs;
        }

        friend std::optional<Register> operator/(Register lhs, Register rhs)
        {
            if (rhs.value == 0)
            {
                return std::nullopt;
            }

            lhs.value /= rhs.value;

            return lhs;
        }

        template <typename U>
        friend std::optional<Register> operator/(Register lhs, U rhs)
        {
            if (rhs == 0)
            {
                return std::nullopt;
            }

            lhs.value /= rhs;

            return lhs;
        }

        friend Register operator&(Register lhs, Register rhs)
        {
            lhs.value &= rhs.value;

            return lhs;
        }

        template <typename U>
        friend Register operator&(Register lhs, U rhs)
        {
            lhs.value &= rhs;

            return lhs;
        }

        friend Register operator|(Register lhs, Register rhs)
        {
            lhs.value |= rhs.value;

            return lhs;
        }

        template <typename U>
        friend Register operator|(Register lhs, U rhs)
        {
            lhs.value |= rhs;

            return lhs;
        }

        friend Register operator^(Register lhs, Register rhs)
        {
            lhs.value ^= rhs.value;

            return lhs;
        }

        template <typename U>
        friend Register operator^(Register lhs, U rhs)
        {
            lhs.value ^= rhs;

            return lhs;
        }

        friend Register operator<<(Register lhs, Register rhs)
        {
            lhs.value <<= rhs.value;

            return lhs;
        }

        template <typename U>
        friend Register operator<<(Register lhs, U rhs)
        {
            lhs.value <<= rhs;

            return lhs;
        }
        friend Register operator>>(Register lhs, Register rhs)
        {
            lhs.value >>= rhs.value;

            return lhs;
        }

        template <typename U>
        friend Register operator>>(Register lhs, U rhs)
        {
            lhs.value >>= rhs;

            return lhs;
        }

        template <typename To>
        [[nodiscard]] To* as()
        {
            return reinterpret_cast<To*>(&value);
        }

        [[nodiscard]] value_type* ptr()
        {
            return &value;
        }

    private:
        value_type value;
    };

    using DataRegister    = Register<std::int32_t, struct DataRegisterTag>;
    using AddressRegister = Register<std::int32_t, struct AddressRegisterTag>;
    using ProgramCounter  = Register<std::uint32_t, struct ProgramCounterTag>;

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

        std::uint8_t extend : 1;   // E / X
        std::uint8_t negative : 1; // N
        std::uint8_t zero : 1;     // Z
        std::uint8_t overflow : 1; // V
        std::uint8_t carry : 1;    // C
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
        ExecutableMemory mem;
        std::optional<TrapType> trap;
    };

    inline momiji::ExecutableMemoryView make_memory_view(System& sys)
    {
        return sys.mem;
    }

    inline momiji::ConstExecutableMemoryView make_memory_view(const System& sys)
    {
        return sys.mem;
    }

} // namespace momiji
