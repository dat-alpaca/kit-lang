#pragma once
#include <vector>
#include <stdexcept>
#include <functional>
#include <unordered_map>

#include "common.hpp"
#include "compiler/segment.hpp"

#include "parser/register.hpp"
#include "parser/opcode.hpp"
#include "parser/instruction.hpp"

#include "instruction_set/copy.hpp"
#include "instruction_set/add.hpp"

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
        { opcode::add, handle_add },
        { opcode::sub, handle_sub },
        { opcode::mul, handle_mul },
        { opcode::out, handle_out },
        { opcode::in, handle_in },
    };

    class compiler
    {
    public:
        explicit compiler()
        {
            // Text section:
            mSections.push_back({ .attributes = segment_attribute::read | segment_attribute::exec });
            
            // Data section:
            mSections.push_back({ .attributes = segment_attribute::read | segment_attribute::write });
        }

    public:
        void compile(const std::vector<kit::instruction>& instructions)
        {
            for (const auto& instruction : instructions)
            {
                if (instruction.code == opcode::section)
                {
                    set_current_section(instruction.operands[0].sectionID);
                    continue;
                }

                if (gOpcodeMap.contains(instruction.code))
                {
                    gOpcodeMap[instruction.code](get_current_section_code(), instruction);
                    continue;
                }
                
                throw std::runtime_error("invalid opcode");
            }
        }

    private:
        void set_current_section(section_id id)
        {
            if (id > mSections.size())
                throw std::runtime_error("invalid section id");

            mCurrentSection = id;
        }

        inline std::vector<u8>& get_current_section_code() { return mSections[mCurrentSection].code; }

    public: 
        std::vector<segment>& get_segments() { return mSections; }

    private:
        std::vector<segment> mSections;
        section_id mCurrentSection = TextSegment;
    };
}