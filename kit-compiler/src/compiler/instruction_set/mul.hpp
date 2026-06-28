#pragma once
#include <vector>
#include "parser/statement.hpp"

namespace kit
{
    class compiler;
    void handle_mul(compiler&, std::vector<u8>& code, const instruction& instruction);
}