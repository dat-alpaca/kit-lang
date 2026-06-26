#pragma once
#include "common.hpp"
#include <vector>

namespace kit
{
    enum class segment_attribute : u8
    {
        none   = 0,
        read   = 1 << 0,
        write  = 1 << 1,
        exec   = 1 << 2,
    };

    inline segment_attribute operator& (const segment_attribute& lhs, const segment_attribute& rhs)
    {
        return static_cast<segment_attribute>(static_cast<u8>(lhs) & static_cast<u8>(rhs));
    }

    inline segment_attribute operator| (const segment_attribute& lhs, const segment_attribute& rhs)
    {
        return static_cast<segment_attribute>(static_cast<u8>(lhs) | static_cast<u8>(rhs));
    }

    struct segment
    {
        std::vector<u8> code;
        segment_attribute attributes = segment_attribute::none;
    };

    constexpr static inline u64 TextSegment = 0;
    constexpr static inline u64 DataSegment = 1;
}