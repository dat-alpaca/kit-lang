#pragma once
#include <vector>
#include "common.hpp"
#include "parser/statement.hpp"
#include "assembler/instruction_set/utils.hpp"

namespace kit
{
    class assembler;

    inline void handle_jb(assembler&, std::vector<u8>& code, const instruction& instruction)
    {
        // JL rel32 (6x bytes)

        u32 nextRIP = static_cast<u32>(code.size() + 6);
        
        code.push_back(0x0F);
        code.push_back(0x8C);

        u32 targetOffset = static_cast<u32>(instruction.operands[0].immediate);
        int32_t rel32 = static_cast<int32_t>(targetOffset) - static_cast<int32_t>(nextRIP);
        
        write_imm32(code, rel32);
    }

    inline void handle_jnb(assembler&, std::vector<u8>& code, const instruction& instruction)
    {
        // JNL rel32 (6x bytes)

        u32 nextRIP = static_cast<u32>(code.size() + 6);
        
        code.push_back(0x0F);
        code.push_back(0x8D);

        u32 targetOffset = static_cast<u32>(instruction.operands[0].immediate);
        int32_t rel32 = static_cast<int32_t>(targetOffset) - static_cast<int32_t>(nextRIP);
        
        write_imm32(code, rel32);
    }
}