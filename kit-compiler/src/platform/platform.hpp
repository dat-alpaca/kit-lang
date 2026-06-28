#pragma once
#include <fstream>
#include <vector>

#include "common.hpp"
#include "compiler/segment.hpp"

namespace kit::platform
{
    constexpr u64 BaseAddress = 0x400000;
    constexpr u64 PageSize = 0x1000;

    void write_executable(std::ofstream& file, std::vector<segment>& segments);
}

namespace kit::platform
{
    void sys_out(std::vector<kit::u8>& code);
}