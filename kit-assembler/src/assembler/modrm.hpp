#pragma once
#include "common.hpp"

namespace kit
{
    enum class mod_field : u8
    {
        no_displacement     = 0b00,
        signed_8bit_addr    = 0b01,
        signed_32bit_addr   = 0b10,
        direct              = 0b11,
    };

    struct mod_rm
    {
    public:
        constexpr u8 value() const
        {
            return (static_cast<u8>(mod) << 6) | (static_cast<u8>(reg) << 3) | rm;
        }

    public:
        u8 rm  : 3;
        u8 reg : 3;
        mod_field mod : 2;
    };

    constexpr static inline u8 sib = 0b100;

    constexpr static inline u8 rax = 0b000;
    constexpr static inline u8 rcx = 0b001;
    constexpr static inline u8 rdx = 0b010; 
    constexpr static inline u8 rbx = 0b011;
    constexpr static inline u8 rsp = 0b100;
    constexpr static inline u8 rbp = 0b101;
    constexpr static inline u8 rsi = 0b110;
    constexpr static inline u8 rdi = 0b111;
}