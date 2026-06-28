#include "out.hpp"
#include "utils.hpp"
#include "compiler/modrm.hpp"
#include "platform/platform.hpp"

using namespace kit;

namespace kit
{
    void handle_out(compiler&, std::vector<kit::u8>& code, const kit::instruction& instruction)
    {
        using namespace kit;

        // sub rsp, 8
        code.insert(code.end(), { 0x48, 0x83, 0xEC, 0x08 });

        code.push_back(0x48); // rex.w
        code.push_back(0x89); // mov r/m64

        u8 register_ = register_from_operand(instruction.operands[0].register_);
        write_mod_rm(code, mod_field::no_displacement, register_, sib);

        code.push_back(0x24); // SIB: rsp base

        platform::sys_out(code);

        // add rsp, 8
        code.insert(code.end(), { 0x48, 0x83, 0xC4, 0x08 });
    }
}