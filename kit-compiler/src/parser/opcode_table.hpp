#pragma once
#include <unordered_map>
#include <string_view>
#include "opcode.hpp"

namespace kit
{
    static inline std::unordered_map<std::string_view, opcode> kOpcodeTable =
    {
        { "copy", opcode::copy },
        { "add", opcode::add },
        { "sub", opcode::sub },
        { "mul", opcode::mul },

        { "in", opcode::in },
        { "out", opcode::out },
    };
}