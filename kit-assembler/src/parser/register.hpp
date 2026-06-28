#pragma once

#include <algorithm>
#include <string_view>
namespace kit
{
    enum class register_k
    {
        invalid, ax, bx, cx, dx 
    };

    static constexpr inline std::string_view get_register_text(register_k register_)
    {
        switch(register_)
        {
            case register_k::ax: return "AX";
            case register_k::bx: return "BX";
            case register_k::cx: return "CX";
            case register_k::dx: return "DX";

            default:
                return "INVALID";
        }
    }

    static constexpr inline register_k get_register(std::string_view name)
    {
        if (std::ranges::equal(name, std::string_view("ax")))
            return register_k::ax;

        else if (std::ranges::equal(name, std::string_view("bx")))
            return register_k::bx;

        else if (std::ranges::equal(name, std::string_view("cx")))
            return register_k::cx;

        else if (std::ranges::equal(name, std::string_view("dx")))
            return register_k::dx;

        return register_k::invalid;
    }
}