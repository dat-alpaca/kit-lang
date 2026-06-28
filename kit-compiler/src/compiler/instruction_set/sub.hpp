#pragma once
#include <vector>
#include "parser/statement.hpp"

namespace kit
{
    void handle_sub(std::vector<u8>& code, const instruction& instruction);
}