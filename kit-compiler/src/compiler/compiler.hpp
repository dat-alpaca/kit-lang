#pragma once
#include <vector>
#include <stdexcept>
#include <functional>
#include <unordered_map>

#include "common.hpp"
#include "compiler/segment.hpp"

#include "parser/register.hpp"
#include "parser/opcode.hpp"
#include "parser/statement.hpp"

#include "instruction_set/copy.hpp"
#include "instruction_set/add.hpp"
#include "instruction_set/jmp.hpp"

namespace
{
    using namespace kit;

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
    using handle_function = std::function<void(std::vector<u8>& code, const instruction& instruction)>;

    static inline std::unordered_map<opcode, handle_function> gOpcodeMap =
    {
        { opcode::copy, handle_copy },
        { opcode::add,  handle_add },
        { opcode::sub,  handle_sub },
        { opcode::mul,  handle_mul },
        { opcode::out,  handle_out },
        { opcode::in,   handle_in },
        { opcode::jmp,  handle_jmp }
    };

    class compiler
    {
    public:
        explicit compiler();

    public:
        void compile(std::vector<statement>& statements);

    private:
        void zeroth_pass(std::vector<statement>& statements);
        void first_pass(std::vector<statement>& statements);
        void second_pass(std::vector<statement>& statements);

    public: 
        std::vector<segment>& get_segments() { return mSegments; }

    private:
        std::unordered_map<std::string_view, u64> mLabelMap;
        std::string_view mCurrentLabel;
        std::string_view mEntryLabel;
        u64 mFirstPassCounter = 0;
    
    private:
        std::vector<segment> mSegments;
        u64 mCurrentSectionID = 0;
    };
}