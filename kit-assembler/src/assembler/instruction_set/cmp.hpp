#pragma once
#include <vector>
#include "parser/statement.hpp"

namespace kit
{
    class assembler;
    void handle_cmp(assembler&, std::vector<u8>& code, const instruction& instruction);
}