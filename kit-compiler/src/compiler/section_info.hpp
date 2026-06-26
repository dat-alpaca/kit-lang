#pragma once
#include <string_view>

#include "common.hpp"
#include "parser/instruction.hpp"


namespace kit
{
    struct section_info
    {
        section_id sectionID;
        std::string_view sectionName;
        
        u64 startingPosition = 0;
        u64 currentOffset = 0;
    };
}