#pragma once
#include <vector>
#include "parser/statement.hpp"

namespace kit
{
    class assembler;
    void handle_in(assembler&, std::vector<u8>& code, const instruction& instruction);
}