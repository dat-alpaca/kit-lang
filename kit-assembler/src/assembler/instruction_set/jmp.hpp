#pragma once
#include <vector>
#include "assembler/instruction_set/utils.hpp"
#include "parser/statement.hpp"

namespace kit
{
    inline void jmp(std::vector<u8>& code, u64 offset, u32 targetAddress)
    {
        code.insert(code.begin() + offset, 0xE9);
        
        u32 nextRIP = static_cast<u32>(offset + 5);
        u32 rel32 = targetAddress - nextRIP;
        
        write_imm32_at(code, offset + 1, rel32);
    }

    class assembler;
    inline void handle_jmp(assembler&, std::vector<u8>& code, const instruction& instruction)
    {
        // JMP near (5x bytes)

        u32 nextRIP = static_cast<u32>(code.size() + 5);
        code.push_back(0xE9);

        u32 targetOffset = static_cast<u32>(instruction.operands[0].immediate);
        int32_t rel32 = static_cast<int32_t>(targetOffset) - static_cast<int32_t>(nextRIP);
        
        write_imm32(code, rel32);
    }
}