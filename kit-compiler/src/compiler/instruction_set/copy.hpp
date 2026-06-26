#pragma once
#include <vector>
#include "common.hpp"
#include "parser/instruction.hpp"

namespace kit
{
    void handle_copy(std::vector<u8>& code, const instruction& instruction);
}