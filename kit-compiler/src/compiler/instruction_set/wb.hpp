#pragma once
#include <vector>
#include "parser/statement.hpp"

namespace kit
{
    class compiler;
    void handle_wb(compiler&, std::vector<u8>& code, const instruction& instruction);
}