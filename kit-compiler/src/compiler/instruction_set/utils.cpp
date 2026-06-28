#include "utils.hpp"
#include <stdexcept>

namespace kit
{
    u8 register_from_operand(register_k register_)
    {
        switch (register_)
        {
            case register_k::ax: return rax;
        }

        throw std::runtime_error("invalid register operand");
    }

    void write_mod_rm(std::vector<kit::u8>& code, mod_field mod, u8 reg, u8 rm)
    {
        mod_rm modrm
        {
            .rm = rm,
            .reg = reg,
            .mod = mod,
        };

        code.push_back(modrm.value());
    }

    void write_imm32_at(std::vector<u8>& code, u64 at, u32 imm32)
    {
        u8 bytes[4];
        for (u8 i = 0; i < 4; ++i)
            bytes[i] = (imm32 >> (8 * i)) & 0xFF;

        code.insert(code.begin() + at, bytes, bytes + 4);
    }

    void write_imm32(std::vector<kit::u8>& code, u32 imm32)
    {
        for (u8 i = 0; i < 4; ++i)
            code.push_back((imm32 >> (8 * i)) & 0xFF);
    }

    void write_imm64(std::vector<kit::u8>& code, u64 imm64)
    {
        for (u8 i = 0; i < 8; ++i)
            code.push_back(static_cast<u8>((imm64 >> (i * 8)) & 0xFF));
    }
}