#pragma once
#include <vector>

#include "common.hpp"
#include "parser/register.hpp"
#include "compiler/modrm.hpp"

namespace kit
{
    u8 register_from_operand(register_k register_);

    void write_mod_rm(std::vector<u8>& code, mod_field mod, u8 reg, u8 rm);

    void write_imm32(std::vector<u8>& code, u32 imm32);
}