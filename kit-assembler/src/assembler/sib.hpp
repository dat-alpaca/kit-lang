#pragma once
#include "common.hpp"

namespace kit
{
    struct sib
    {
    public:
        constexpr u8 value() const
        {
            return *reinterpret_cast<const u8*>(this);
        }

    public:
        u8 base  : 3;
        u8 index : 3;
        u8 scale : 2;
    };

    constexpr static inline u8 sib_no_index = 0b100;
    constexpr static inline u8 sib_no_base  = 0b101;
}