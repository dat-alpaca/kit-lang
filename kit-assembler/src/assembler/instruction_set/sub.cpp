#include <stdexcept>
#include "sub.hpp"
#include "parser/register.hpp"
#include "utils.hpp"

using namespace kit;

static void handle_sub_register_to_register(std::vector<u8>& code, const instruction& instruction)
{
    // SUB r64, r/m64
    code.push_back(0x48); // rex.w
    code.push_back(0x2B); // SUB r64, r/m64

    u8 registerDst = register_from_operand(instruction.operands[0].register_);
    u8 registerSrc = register_from_operand(instruction.operands[1].register_);

    write_mod_rm(code, mod_field::direct, registerDst, registerSrc);
}

static void handle_sub_imm_to_register(std::vector<u8>& code, const instruction& instruction)
{
    u32 immediate = (u32)instruction.operands[1].immediate;
    u8 register_ = register_from_operand(instruction.operands[0].register_);

    // SUB RAX, imm32
    if (register_ == static_cast<u8>(register_k::ax))
    {
        code.push_back(0x48); // rex.w
        code.push_back(0x2D); // sub rax, imm32

        write_imm32(code, immediate);
        return;
    }

    // SUB r/m64, imm32
    code.push_back(0x48); // rex.w
    code.push_back(0x81); // sub r/m64, imm32

    write_mod_rm(code, mod_field::direct, 5, register_);
    write_imm32(code, immediate);
}

namespace kit
{
    void handle_sub(assembler&, std::vector<u8>& code, const instruction& instruction)
    {
        switch(instruction.operands[0].type)
        {
            case operand::kind::register_:
            {
                switch (instruction.operands[1].type)
                {
                    case operand::kind::register_:
                        return handle_sub_register_to_register(code, instruction);

                    case operand::kind::immediate:
                        return handle_sub_imm_to_register(code, instruction);
                }
            } break;

            default:
                throw std::runtime_error("invalid mul instruction. must use register as operand 0");
        }
    }
}
