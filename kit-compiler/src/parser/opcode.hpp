#pragma once

namespace kit
{
    enum class opcode
    {
        invalid = 0,
        section,
        
        copy,
        add,
        sub,
        mul,

        in,
        out,
    };

    inline bool opcode_has_operands(opcode code)
    {
        switch (code)
        {
            case opcode::section:
            case opcode::copy:
            case opcode::add:
            case opcode::sub:
            case opcode::mul:
            case opcode::in:
            case opcode::out:
                return true;
            
            default:
                return false;
        }
    }

    static inline const char* get_opcode_text(opcode code)
    {
        switch(code)
        {
            case opcode::section: return "section";
            case opcode::copy: return "copy";
            case opcode::add: return "add";
            case opcode::sub: return "sub";
            case opcode::mul: return "mul";
            case opcode::in: return "in";
            case opcode::out: return "out";

            default:
                return "invalid";
        }
    }
}