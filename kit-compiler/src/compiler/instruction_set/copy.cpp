#include <stdexcept>

#include "copy.hpp"
#include "utils.hpp"

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

static void handle_copy_address_value_to_register(std::vector<kit::u8>& code, const kit::instruction& instruction)
{
}

namespace kit
{
    void handle_copy(std::vector<u8>& code, const instruction& instruction)
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

                    case operand::kind::pointer:
                        return handle_copy_address_value_to_register(code, instruction);
                }
            } break;

            case operand::kind::pointer:
                throw std::runtime_error("copy [mem], r/m64 not implemented");

            default:
                throw std::runtime_error("invalid copy instruction. must use register or pointer as operand 0");
        }
    }
}