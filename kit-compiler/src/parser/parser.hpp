#pragma once
#include <span>
#include <vector>

#include "lexer/token.hpp"
#include "statement.hpp"

namespace kit
{
    class parser
    {
    public:
        explicit parser(std::span<const token> tokens);

    public:
        void parse();

    private:
        label parse_label();
        section parse_section_directive();
        entry parse_entry_directive();

        instruction parse_instruction();
        opcode parse_opcode();
        operand parse_operand();

        bool is_label() const;
        bool is_section_directive() const;
        bool is_entry_directive() const;

    private:
        bool valid_peek(u64 offset = 0) const;
        const token& peek(u64 offset= 0) const;
        const token& advance();
        bool match(token_kind kind);

    public:
        inline std::vector<statement>& get_statements() { return mStatements; }

    private:
        std::vector<statement> mStatements;
        std::span<const token> mTokens;
        u64 mCurrent = 0;
    };
}