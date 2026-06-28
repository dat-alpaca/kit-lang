#pragma once
#include <vector>
#include <functional>
#include <unordered_map>

#include "common.hpp"
#include "compiler/segment.hpp"

#include "parser/opcode.hpp"
#include "parser/statement.hpp"

#include "instruction_set/copy.hpp"
#include "instruction_set/add.hpp"
#include "instruction_set/jmp.hpp"
#include "instruction_set/out.hpp"
#include "instruction_set/sub.hpp"
#include "instruction_set/mul.hpp"
#include "instruction_set/in.hpp"

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

    private:
        void resolve_instruction_operands(instruction& instruction);

    public: 
        std::vector<segment>& get_segments() { return mSegments; }

    private:
        std::unordered_map<std::string_view, u64> mLabelMap;
        std::unordered_map<std::string_view, u64> mMemoryOperandMap;
        std::string_view mCurrentLabel;
        std::string_view mEntryLabel;
    
    private:
        std::vector<segment> mSegments;
        std::vector<u64> mSegmentPCs;
        u64 mCurrentSectionID = TextSegment;
    };
}