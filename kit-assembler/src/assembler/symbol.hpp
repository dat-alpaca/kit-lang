#pragma once
#include "common.hpp"
#include <string_view>

namespace kit
{
    struct symbol
    {
        std::string_view name;
        u64 virtualAddress;
        u64 size;
    };
}