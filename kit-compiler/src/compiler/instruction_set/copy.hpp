#pragma once
#include <vector>
#include "common.hpp"
#include "parser/statement.hpp"

namespace kit
{
    u64 get_copy_instruction_size(const instruction& instruction);

    class compiler;
    void handle_copy(compiler&, std::vector<u8>& code, const instruction& instruction);
}