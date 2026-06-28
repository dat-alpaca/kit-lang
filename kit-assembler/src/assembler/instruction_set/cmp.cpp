#include <stdexcept>
#include "cmp.hpp"
#include "assembler/instruction_set/utils.hpp"
#include "assembler/modrm.hpp"
#include "assembler/sib.hpp"
#include "parser/statement.hpp"
#include "assembler/assembler.hpp"

using namespace kit;

static void handle_cmp_mem_to_immediate(assembler&, std::vector<u8>& code, const instruction& instruction)
{
    // CMP r/m64, imm32

    u8 register_ = register_from_operand(instruction.operands[0].register_);
    u32 immediate = (u32)instruction.operands[1].immediate;

    code.push_back(0x48); // rex.w
    code.push_back(0x81); // cmp r/m64, imm32

    write_mod_rm(code, mod_field::direct, 0b111, register_);
    write_imm32(code, immediate);
}

static void handle_cmp_mem_to_register(assembler& assembler, std::vector<u8>& code, const instruction& instruction)
{
    // CMP r64, r/m64

    u8 register_ = register_from_operand(instruction.operands[0].register_);
    u64 address = instruction.operands[1].immediate;

    u64 rex = 0x48;         // rex.w
    if (register_ > rdi) 
        rex |= 0x04;        // extended registers

    code.push_back(rex); 
    code.push_back(0x3B);   // CMP r64, r/m64

    write_mod_rm_sib(code, mod_field::no_displacement, register_);

    // SIB:
    sib sibByte = 
    {
        .base  = sib_no_base,
        .index = sib_no_index,
        .scale = 0b00,
    };
    code.push_back(sibByte.value());

    u32 placeholder = 0;
    write_imm32(code, placeholder);

    assembler.insert_reallocation
    ({ 
        .codeByteOffset = code.size() - sizeof(u32), 
        .dataByteOffset = address,
        .size = sizeof(u32)
    });
}

namespace kit
{
    void handle_cmp(assembler& assembler, std::vector<u8>& code, const instruction& instruction)
    {
        switch(instruction.operands[0].type)
        {
            case operand::kind::register_:
            {
                switch (instruction.operands[1].type)
                {
                    case operand::kind::immediate:
                        return handle_cmp_mem_to_immediate(assembler, code, instruction);

                    case operand::kind::memory:
                        return handle_cmp_mem_to_register(assembler, code, instruction);

                    default:
                        throw std::runtime_error("invalid add instruction. must use register as operand 0");
                }
            } break;

            default:
                throw std::runtime_error("invalid add instruction. must use register as operand 0");
        }
    }
}