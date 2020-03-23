#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <optional>
#include <string_view>

#include <momiji/Types.h>
#include <momiji/Parser/Errors.h>
#include <momiji/Parser/Operands.h>

#include <gsl/span>
#include <asl/meta>

namespace momiji::details
{
    template <typename T>
    using validate_t = decltype(std::declval<const T&>().validate());

    template <typename T>
    using encode_t = decltype(std::declval<T&>().encode());

    template <typename T>
    using operands_t = decltype(std::declval<T&>().operands());

    template <typename T>
    using const_operands_t = decltype(std::declval<const T&>().operands());

    template <typename T>
    inline constexpr bool is_instruction = asl::require<
        asl::exists<encode_t>
    >;
}

namespace momiji::parser
{
    struct Instruction
    {
        Instruction();

        template <typename T,
                  typename = std::enable_if_t<details::is_instruction<T>>>
        Instruction(T&& instruction)
        {
            new (m_objectBuffer.data()) Model<T>(std::forward<T>(instruction));
        }

        ~Instruction();

        Instruction(const Instruction&);
        Instruction& operator=(const Instruction&);
        Instruction(Instruction&&) noexcept;
        Instruction& operator=(Instruction&&) noexcept;


        [[nodiscard]] std::int16_t encode() const;
        [[nodiscard]] bool validate() const;

        [[nodiscard]] gsl::span<momiji::Operand> operands();
        [[nodiscard]] gsl::span<const momiji::Operand> operands() const;

    private:

        constexpr static auto maxObjectSize = sizeof(std::vector<Operand>);

        struct Concept
        {
            Concept() = default;
            virtual ~Concept() = default;

            Concept(const Concept&) = default;
            Concept& operator=(const Concept&) = default;
            Concept(Concept&&) = default;
            Concept& operator=(Concept&&) = default;

            [[nodiscard]] virtual std::optional<momiji::ParserError> validate() const = 0;
            [[nodiscard]] virtual gsl::span<momiji::Operand> operands() const = 0;
            virtual gsl::span<momiji::Operand> operands() = 0;
        };

        template <typename T>
        struct Model final : Concept
        {
            template <typename U,
                      typename = std::enable_if_t<std::is_convertible_v<U, T>>>
            Model(U&& u)
                : t(std::forward(u))
            {
            }

            ~Model() override = default;
            Model(const Model&) = default;
            Model& operator=(const Model&) = default;
            Model(Model&&) noexcept = default;
            Model& operator=(Model&&) noexcept = default;

            T t;
        };

        std::array<char, maxObjectSize> m_objectBuffer;

        Concept* underlyingObject();

    public:

        std::int32_t programCounter; // NOLINT
        std::int32_t sourceLine; // NOLINT
        momiji::DataType dataType : 2; // NOLINT
    };

    struct IllegalInstruction
    {
        [[nodiscard]] std::optional<ParserError> validate() const;

        std::int16_t encode();

        void operands();
        void operands() const;
    };

    inline void foo()
    {
        IllegalInstruction instr{};
        momiji::parser::Instruction t{instr};
    }

    /*
    struct Instruction
    {
        std::vector<Operand> operands;
        std::int32_t programCounter;
        std::int32_t sourceLine;
        momiji::DataType dataType : 2;
    };
    */
}
