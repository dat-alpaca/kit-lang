#pragma once
#include <vector>

#include "common.hpp"
#include "opcode.hpp"
#include "register.hpp"

namespace kit
{
    using symbol_id = u64;
    using section_id = u64;

    struct operand
    {
        enum class kind { register_, immediate, pointer, section };
        kind type;

        union
        {
            register_k register_;
            u64 immediate;
            symbol_id symbolID;
            section_id sectionID;
        };
    };

    struct instruction
    {
        opcode code;
        std::vector<operand> operands;
    };
}