#include "out.hpp"
#include "utils.hpp"
#include "compiler/modrm.hpp"
#include "platform/platform.hpp"

using namespace kit;

namespace kit
{
    void handle_in(compiler&, std::vector<kit::u8>& code, const kit::instruction& instruction)
    {
        using namespace kit;

        // sub rsp, 8
        code.insert(code.end(), { 0x48, 0x83, 0xEC, 0x08 });

        platform::sys_in(code);
        
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