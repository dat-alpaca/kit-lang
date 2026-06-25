#pragma once
#include <span>
#include <vector>

#include "lexer/token.hpp"
#include "instruction.hpp"

namespace kit
{
    class parser
    {
    public:
        explicit parser(std::span<const token> tokens);

    public:
        std::vector<instruction> parse();

    private:
        instruction parse_instruction();
        opcode parse_opcode();
        operand parse_operand();

    private:
        const token& peek() const;
        const token& advance();
        bool match(token_kind kind);

    private:
        std::span<const token> mTokens;
        u64 mCurrent = 0;
    };
}