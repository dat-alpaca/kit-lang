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
        symbol_id insert_symbol(std::string_view symbolName);
        section_id get_section_from_name(std::string_view sectionName);

    public:
        std::vector<std::string>& get_symbols() { return mSymbolTable; }

    private:
        std::vector<std::string> mSymbolTable;
        std::span<const token> mTokens;
        u64 mCurrent = 0;
    };
}