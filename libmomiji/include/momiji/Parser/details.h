#pragma once

#include <type_traits>
#include <cstddef>

#include <momiji/Parser/AST.h>

namespace momiji::details
{
    template <typename T, typename = void>
    struct has_reg_var : std::false_type
    {
    };

    template <typename T>
    struct has_reg_var<T, std::void_t<decltype(std::declval<T>().reg)>>
        : std::true_type
    {
    };

    template <typename T, typename = void>
    struct has_value_var : std::false_type
    {
    };

    template <typename T>
    struct has_value_var<T, std::void_t<decltype(std::declval<T>().value)>>
        : std::true_type
    {
    };

    struct RegVisitor
    {
        template <typename T>
        void operator()(const T& reg)
        {
            if constexpr (details::has_reg_var<T>::value)
            {
                val = reg.reg;
            }
        }

        std::int32_t val { 0 };
    };

    struct ValueVisitor
    {
        ValueVisitor(const LabelInfo& labels)
            : labels(labels)
        {
        }

        template <typename T>
        void operator()(const T& value)
        {
            if constexpr (details::has_value_var<T>::value)
            {
                val = momiji::resolveAST(*value.value, labels);
            }
        }

        std::int32_t val { 0 };
        const LabelInfo& labels;
    };
}
