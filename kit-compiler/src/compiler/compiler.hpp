#pragma once
#include <stdexcept>
#include <vector>

#include "common.hpp"
#include "parser/instruction.hpp"
#include "parser/opcode.hpp"
#include "parser/register.hpp"

namespace
{
    inline void handle_copy(std::vector<kit::u8>& code, const kit::instruction& instruction)
    {
        using namespace kit;

        code.push_back(0x48); // rex.w
        code.push_back(0xC7); // mov r/m64, imm32

        switch(instruction.operands[0].register_)
        {
            case register_k::ax:
                code.push_back(0xC0);
                break;

            default:
                throw std::runtime_error("compilation failed: invalid mov register");
        }

        u32 immediate = (u32)instruction.operands[1].immediate;
        for (int i = 0; i < 4; ++i)
            code.push_back((immediate >> (8 * i)) & 0xFF);
    }

    inline void handle_add(std::vector<kit::u8>& code, const kit::instruction& instruction)
    {
        using namespace kit;

        code.push_back(0x48); // rex.w
        code.push_back(0x05); // add rax, imm32

        u32 immediate = (u32)instruction.operands[1].immediate;
        for (int i = 0; i < 4; ++i)
            code.push_back((immediate >> (8 * i)) & 0xFF);
    }

    inline void handle_sub(std::vector<kit::u8>& code, const kit::instruction& instruction)
    {
        using namespace kit;

        code.push_back(0x48); // rex.w
        code.push_back(0x2D); // sub rax, imm32

        u32 immediate = (u32)instruction.operands[1].immediate;
        for (int i = 0; i < 4; ++i)
            code.push_back((immediate >> (8 * i)) & 0xFF);
    }

    inline void handle_mul(std::vector<kit::u8>& code, const kit::instruction& instruction)
    {
        
    }

    // TO-DO: Platform-dependence:
    inline void handle_out(std::vector<kit::u8>& code, const kit::instruction& instruction)
    {
        using namespace kit;

        // sub rsp, 8
        code.insert(code.end(), { 0x48, 0x83, 0xEC, 0x08 });

        // mov [rsp], <register>
        switch(instruction.operands[0].register_)
        {
            case register_k::ax:
                code.insert(code.end(), { 0x48, 0x89, 0x04, 0x24 });
                break;

            default:
                throw std::runtime_error("compilation failed: invalid out register");
        }

        code.insert(code.end(),
        {
            0xB8, 0x01, 0x00, 0x00, 0x00,           // mov eax, 1
            0xBF, 0x01, 0x00, 0x00, 0x00,           // mov edi, 1
            0x48, 0x89, 0xE6,                       // mov rsi, rsp
            0xBA, 0x08, 0x00, 0x00, 0x00,           // mov edx, 8
            0x0F, 0x05,                             // syscall                  
        });

        // add rsp, 8
        code.insert(code.end(), { 0x48, 0x83, 0xC4, 0x08 });
    }

    // TO-DO: Platform-dependence:
    inline void handle_in(std::vector<kit::u8>& code, const kit::instruction& instruction)
    {
        using namespace kit;

        // sub rsp, 8
        code.insert(code.end(), { 0x48, 0x83, 0xEC, 0x08 });

        code.insert(code.end(),
        {
            0xB8, 0x00, 0x00, 0x00, 0x00,           // mov eax, 0
            0xBF, 0x00, 0x00, 0x00, 0x00,           // mov edi, 0
            0x48, 0x89, 0xE6,                       // mov rsi, rsp
            0xBA, 0x08, 0x00, 0x00, 0x00,           // mov edx, 8
            0x0F, 0x05,                             // syscall                  
        });

        // movzx <register>, byte ptr [rsp]
        switch(instruction.operands[0].register_)
        {
            case register_k::ax:
                code.insert(code.end(), { 0x48, 0x0F, 0xB6, 0x04, 0x24 });
                break;

            default:
                throw std::runtime_error("compilation failed: invalid out register");
        }

        // add rsp, 8
        code.insert(code.end(), { 0x48, 0x83, 0xC4, 0x08 });
    }
}

namespace kit
{
    inline std::vector<u8> compile(const std::vector<instruction>& instructions)
    {
        std::vector<u8> code;

        for (const auto& instruction : instructions)
        {
            switch (instruction.code)
            {
                case opcode::copy:
                    handle_copy(code, instruction);
                    break;

                case opcode::add:
                    handle_add(code, instruction);
                    break;

                case opcode::sub:
                    handle_sub(code, instruction);
                    break;

                case opcode::mul:
                    handle_mul(code, instruction);
                    break;

                case opcode::in:
                    handle_in(code, instruction);
                    break;

                case opcode::out:
                    handle_out(code, instruction);
                    break;

                default:
                    throw std::runtime_error("failed to compile");
            }
        }

        return code;
    }
}