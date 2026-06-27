#pragma once
#include <string>
#include <variant>
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
        enum class kind { register_, immediate, label };
        kind type;

        union
        {
            register_k register_;
            u64 immediate;
        };

        std::string_view label;
    };

    struct instruction
    {
        opcode code;
        std::vector<operand> operands;
    };

    struct label
    {
        std::string_view name;
    };

    struct section
    {
        std::string_view name;
    };

    struct entry
    {
        std::string_view name;
    };

    using statement = std::variant<instruction, label, section, entry>;
}