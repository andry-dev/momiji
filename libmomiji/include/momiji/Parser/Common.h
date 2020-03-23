#pragma once

#include <cstddef>
#include <vector>
#include <string>

namespace momiji
{
    struct Breakpoint
    {
        std::int64_t source_line { 0 };
    };

    struct Label
    {
        Label(std::uint32_t nameHash, std::int64_t idx, std::string_view string)
            : string(string)
            , idx(idx)
            , nameHash(nameHash)
        {
        }

        Label()  = default;
        ~Label() = default;

        Label(const Label&) = default;
        Label& operator=(const Label&) = default;

        Label(Label&&) = default;
        Label& operator=(Label&&) = default;

        std::string string {};
        std::int64_t idx { 0 };
        std::uint32_t nameHash { 0 };
    };

    using LabelInfo = std::vector<momiji::Label>;
}
