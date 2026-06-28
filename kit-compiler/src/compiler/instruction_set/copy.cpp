#include <stdexcept>
#include <vector>

#include "copy.hpp"
#include "parser/statement.hpp"
#include "utils.hpp"
#include "compiler/compiler.hpp"

using namespace kit;

static void handle_copy_register_to_register(std::vector<u8>& code, const instruction& instruction)
{
    // MOV r/m64, r64
    code.push_back(0x48); // rex.w
    code.push_back(0x89); // mov r/m64, r64

    u8 registerSource = register_from_operand(instruction.operands[1].register_);
    u8 registerDest = register_from_operand(instruction.operands[0].register_);

    write_mod_rm(code, mod_field::direct, registerDest, registerSource);
}

static void handle_copy_imm_to_register(std::vector<u8>& code, const instruction& instruction)
{
    // MOV r/m64, imm32
    code.push_back(0x48); // rex.w
    code.push_back(0xC7); // mov r/m64, imm32

    u8 register_ = register_from_operand(instruction.operands[0].register_);
    write_mod_rm(code, mod_field::direct, 0b000, register_);

    // imm32
    u32 immediate = (u32)instruction.operands[1].immediate;
    write_imm32(code, immediate);
}

static void handle_copy_mem_to_register(compiler& compiler, std::vector<u8>& code, const instruction& instruction)
{
    u8 register_ = register_from_operand(instruction.operands[0].register_);
    u64 address = instruction.operands[1].immediate;

    // MOV RAX, moffs643
    if (register_ == rax)
    {
        code.push_back(0x48); // rex.w
        code.push_back(0xA1); // MOV RAX, moffs643
        
        u64 placeholder = 0;
        write_imm64(code, placeholder);

        compiler.insert_reallocation
        ({ 
            .codeByteOffset = code.size() - sizeof(u64), 
            .dataByteOffset = address
         });
    }
    else
    {
        throw std::runtime_error("MOV r64, r/m64 not implemented");
        /* implement */
        code.push_back(0x48); // rex.w
        code.push_back(0x8B); // MOV r64, r/m64
    }
}

namespace kit
{
    void handle_copy(compiler& compiler, std::vector<u8>& code, const instruction& instruction)
    {
        switch(instruction.operands[0].type)
        {
            case operand::kind::register_:
            {
                switch (instruction.operands[1].type)
                {
                    case operand::kind::register_:
                        return handle_copy_register_to_register(code, instruction);

                    case operand::kind::immediate:
                        return handle_copy_imm_to_register(code, instruction);

                    case operand::kind::memory:
                        return handle_copy_mem_to_register(compiler, code, instruction);
                }
            } break;

            default:
                throw std::runtime_error("invalid copy instruction. must use register as operand 0");
        }
    }
}