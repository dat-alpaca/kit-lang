#pragma once
#include <string_view>
#include <cctype>
#include <ranges>

namespace kit
{
    inline auto to_lower_view(std::string_view stringView)
    {
        return stringView | std::views::transform([](unsigned char c) -> char
        {
            return static_cast<char>(std::tolower(c));
        });
    }
}