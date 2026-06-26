#pragma once
#include <vector>
#include "parser/instruction.hpp"

namespace kit
{
    void handle_add(std::vector<u8>& code, const instruction& instruction);
}