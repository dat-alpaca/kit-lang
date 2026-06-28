#include <stdexcept>
#include "add.hpp"
#include "assembler/assembler.hpp"
#include "parser/register.hpp"
#include "utils.hpp"

using namespace kit;

static void handle_add_register_to_register(std::vector<u8>& code, const instruction& instruction)
{
    // ADD r64, r/m64
    code.push_back(0x48); // rex.w
    code.push_back(0x03); // ADD r64, r/m64

    u8 registerDst = register_from_operand(instruction.operands[0].register_);
    u8 registerSrc = register_from_operand(instruction.operands[1].register_);

    write_mod_rm(code, mod_field::direct, registerDst, registerSrc);
}

static void handle_add_imm_to_register(std::vector<u8>& code, const instruction& instruction)
{
    u32 immediate = (u32)instruction.operands[1].immediate;
    u8 register_ = register_from_operand(instruction.operands[0].register_);

    // ADD RAX, imm32
    if (register_ == static_cast<u8>(register_k::ax))
    {
        code.push_back(0x48); // rex.w
        code.push_back(0x05); // add rax, imm32

        write_imm32(code, immediate);
        return;
    }

    // ADD r/m64, imm32
    code.push_back(0x48); // rex.w
    code.push_back(0x81); // add r/m64, imm32

    write_mod_rm(code, mod_field::direct, 0b000, register_);
    write_imm32(code, immediate);
}

static void handle_add_mem_to_register(assembler& assembler, std::vector<u8>& code, const instruction& instruction)
{
    u8 register_ = register_from_operand(instruction.operands[0].register_);
    u64 address = instruction.operands[1].immediate;

    // ADD RAX, moffs643
    if (register_ == rax)
    {
        code.push_back(0x48); // rex.w
        code.push_back(0xA1); // MOV RAX, moffs643
        
        u64 placeholder = 0;
        write_imm64(code, placeholder);

        assembler.insert_reallocation
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
    void handle_add(assembler&, std::vector<u8>& code, const instruction& instruction)
    {
        switch(instruction.operands[0].type)
        {
            case operand::kind::register_:
            {
                switch (instruction.operands[1].type)
                {
                    case operand::kind::register_:
                        return handle_add_register_to_register(code, instruction);

                    case operand::kind::immediate:
                        return handle_add_imm_to_register(code, instruction);
                }
            } break;

            default:
                throw std::runtime_error("invalid add instruction. must use register as operand 0");
        }
    }
}