#pragma once
#include <array>
#include <string_view>

namespace kit
{
    enum class opcode
    {
        invalid = 0,
        wb,
        copy,
        add,
        sub,
        mul,
        in,
        out,
        jmp,
        jb,
        jnb,
        cmp,
    };

    static inline constexpr std::array kOpcodeTable
    {
        std::pair{ "wb"  , opcode::wb   },
        std::pair{ "copy", opcode::copy },
        std::pair{ "add" , opcode::add  },
        std::pair{ "sub" , opcode::sub  },
        std::pair{ "mul" , opcode::mul  },
        std::pair{ "in"  , opcode::in   },
        std::pair{ "out" , opcode::out  },
        std::pair{ "jmp" , opcode::jmp  },
        std::pair{ "jb"  , opcode::jb   },
        std::pair{ "jnb" , opcode::jnb  },
        std::pair{ "cmp" , opcode::cmp  },
    };

    constexpr std::string_view get_opcode_text(opcode op)
    {
        for (auto [name, value] : kOpcodeTable)
            if (value == op)
                return name;

        return "invalid";
    }
}