#pragma once
#include <fstream>
#include <vector>

#include "common.hpp"

namespace kit::platform
{
    void write_executable(std::ofstream& file, std::vector<u8> code);
}