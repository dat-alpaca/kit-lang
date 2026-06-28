#pragma once

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
    };

    static inline const char* get_opcode_text(opcode code)
    {
        switch(code)
        {
            case opcode::wb: return "wb";
            case opcode::copy: return "copy";
            case opcode::add: return "add";
            case opcode::sub: return "sub";
            case opcode::mul: return "mul";
            case opcode::in: return "in";
            case opcode::out: return "out";
            case opcode::jmp: return "jmp";

            default:
                return "invalid";
        }
    }
}