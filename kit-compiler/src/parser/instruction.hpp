#pragma once
#include <vector>

#include "common.hpp"
#include "opcode.hpp"
#include "register.hpp"

namespace kit
{
    struct operand
    {
        enum class kind { register_, immediate };
        kind type;

        union
        {
            register_k register_;
            u64 immediate;
        };
    };

    struct instruction
    {
        opcode code;
        std::vector<operand> operands;
    };
}