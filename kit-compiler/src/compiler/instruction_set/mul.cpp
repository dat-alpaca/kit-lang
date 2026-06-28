#include <stdexcept>
#include "mul.hpp"
#include "utils.hpp"

using namespace kit;

static void handle_mul_register_to_register(std::vector<u8>& code, const instruction& instruction)
{
    // MUL r/m64
    // AX := AX * r/m64
    code.push_back(0x48); // rex.w
    code.push_back(0xF7); // ADD r64, r/m64

    u8 register_ = register_from_operand(instruction.operands[0].register_);

    write_mod_rm(code, mod_field::direct, 0b010, register_);
}

namespace kit
{
    void handle_mul(compiler&, std::vector<u8>& code, const instruction& instruction)
    {
        switch(instruction.operands[0].type)
        {
            case operand::kind::register_:
            {
                switch (instruction.operands[1].type)
                {
                    case operand::kind::register_:
                        return handle_mul_register_to_register(code, instruction);

                    case operand::kind::label:
                        /* Implement */
                        break;

                    default:
                        throw std::runtime_error("invalid mul instruction");
                }
            } break;

            default:
                throw std::runtime_error("invalid mul instruction. must use register as operand 0");
        }
    }
}