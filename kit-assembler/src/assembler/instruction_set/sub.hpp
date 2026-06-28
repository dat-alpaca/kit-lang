#pragma once
#include <vector>
#include "parser/statement.hpp"

namespace kit
{
    class assembler;
    void handle_sub(assembler&, std::vector<u8>& code, const instruction& instruction);
}