#pragma once
#include <vector>
#include "parser/statement.hpp"

namespace kit
{
    class assembler;
    void handle_add(assembler&, std::vector<u8>& code, const instruction& instruction);
}