#pragma once

namespace kit
{
    enum class register_k
    {
        ax,
    };

    static inline const char* get_register_text(register_k register_)
    {
        switch(register_)
        {
            case register_k::ax: return "AX";

            default:
                return "INVALID";
        }
    }
}