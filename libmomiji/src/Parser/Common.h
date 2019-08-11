#pragma once

#include <Parser.h>
#include <string_view>
#include <vector>

namespace momiji
{
#if 0
    namespace v1
    {
        struct parser_metadata
        {
            bool result;
            std::string_view rest;
            std::string_view parsed_str;
            ParserError::ErrorType error { errors::UnknownError {} };
        };

        struct Label
        {
            Label(std::uint32_t name_hash,
                  std::int64_t idx,
                  std::string_view string)
                : name_hash(name_hash)
                , idx(idx)
                , string(string)
            {
            }

            Label()  = default;
            ~Label() = default;

            Label(const Label&) = default;
            Label& operator=(const Label&) = default;

            Label(Label&&) = default;
            Label& operator=(Label&&) = default;

            std::uint32_t name_hash = 0;
            std::int64_t idx        = 0;
            std::string_view string;
        };

        using LabelInfo = std::vector<momiji::Label>;

        namespace alg
        {
            inline auto find_label(const LabelInfo& labels, std::uint32_t hash)
            {
                auto begin = std::begin(labels);
                auto end   = std::end(labels);

                auto it =
                    std::find_if(begin, end, [&](const momiji::v1::Label& x) {
                        return x.name_hash == hash;
                    });

                return it;
            }
        } // namespace alg
    }     // namespace v1

#endif

    inline namespace v2
    {
        struct parser_metadata
        {
            bool result;
            std::string_view rest;
            std::string_view parsed_str;
            ParserError::ErrorType error { errors::UnknownError {} };
        };
    } // namespace v2
} // namespace momiji
