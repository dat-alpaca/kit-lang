#pragma once
#include "common.hpp"

namespace kit
{
    inline u64 align_address(u64 address, u64 alignment)
    {
        return (address + (alignment - 1)) & ~(alignment - 1); 
    }
}