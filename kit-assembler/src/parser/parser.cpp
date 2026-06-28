#include <algorithm>
#include <stdexcept>
#include <string_view>

#include "parser.hpp"
#include "opcode.hpp"
#include "lexer/token.hpp"

#include "parser/statement.hpp"
#include "parser/opcode.hpp"
#include "parser/register.hpp"
#include "utils/string_utils.hpp"

namespace kit
{
    kit::parser::parser(std::span<const token> tokens)
        : mTokens(tokens)
    {

    }

    void parser::parse()
    {
        mStatements.clear();

        while(peek().kind != token_kind::eof)
        {
            if (is_label())
                mStatements.push_back(parse_label());

            else if (is_section_directive())
                mStatements.push_back(parse_section_directive());

            else if (is_entry_directive())
                mStatements.push_back(parse_entry_directive());

            else
                mStatements.push_back(parse_instruction());
        }
    }
}

namespace kit
{
    label parser::parse_label()
    {
        token current = peek();
        advance();
        advance();  // skip :

        return { .name = current.lexeme };
    }

    section parser::parse_section_directive()
    {
        advance();                  // consumes "section"
        token current = peek();
        advance();                  // consumes the section name

        return { .name = current.lexeme };
    }

    entry parser::parse_entry_directive()
    {
        advance();                  // consumes "entry"

        token current = peek();
        advance();                  // consumes the entry name

        return { .name = current.lexeme };
    }

    instruction parser::parse_instruction()
    {
        instruction instruction = { .code = parse_opcode() };
        instruction.operands.push_back(parse_operand());

        while (match(token_kind::comma))
            instruction.operands.push_back(parse_operand());

        return instruction;
    }

    opcode parser::parse_opcode()
    {
        const auto& current = peek();

        opcode code = opcode::invalid;
        for (auto& [lexeme, opcode] : kOpcodeTable)
        {
            auto lower = to_lower_view(current.lexeme);
            if (!std::ranges::equal(lower, kit::to_lower_view(lexeme)))
                continue;

            code = opcode;
            break;
        }

        if (code == opcode::invalid)
            throw std::runtime_error("invalid opcode parsed");

        advance();
        return code;
    }

    operand parser::parse_operand()
    {
        const auto& current = peek();

        advance();

        switch (current.kind)
        {
            case token_kind::register_:
            {
                register_k reg;
                auto lower = to_lower_view(current.lexeme);

                if (std::ranges::equal(lower, std::string_view("ax")))
                    reg = register_k::ax;
                else
                    throw std::runtime_error("failed to parse operands: unknown register");

                return {
                    .type = operand::kind::register_,
                    .register_ = reg,
                };
            }

            case token_kind::integer:
            {
                return {
                    .type = operand::kind::immediate,
                    .immediate = std::stoull(std::string(current.lexeme))
                };
            }

            case token_kind::identifier:
            {
                if (current.lexeme.starts_with('['))
                {
                    auto lexeme = current.lexeme;
                    lexeme.remove_prefix(1);
                    lexeme.remove_suffix(1);

                    return {
                        .type = operand::kind::memory,
                        .label = lexeme,
                    };
                }

                return {
                    .type = operand::kind::label,
                    .label = current.lexeme,
                };
            }

            default:
                throw std::runtime_error("failed to parse operands: invalid token kind");
        }
    }

    bool parser::is_label() const
    {
        if (!valid_peek(1))
            return false;

        token current = peek();
        token next = peek(1);

        return current.kind == token_kind::identifier && next.kind == token_kind::colon;
    }

    bool parser::is_memory_identifier() const
    {
        token current = peek();
        return (current.kind == token_kind::identifier) && current.lexeme.starts_with('[');
    }

    bool parser::is_section_directive() const
    {
        token current = peek();
        
        auto lower = to_lower_view(current.lexeme);
        bool isNamedSection = std::ranges::equal(lower, std::string_view("section"));
        
        return (current.kind == token_kind::identifier) && isNamedSection;
    }

    bool parser::is_entry_directive() const
    {
        token current = peek();
        
        auto lower = to_lower_view(current.lexeme);
        bool isNamedEntry = std::ranges::equal(lower, std::string_view("entry"));
        
        return (current.kind == token_kind::identifier) && isNamedEntry;
    }
}

namespace kit
{
    bool parser::valid_peek(u64 offset) const
    {
        return offset + mCurrent < mTokens.size();
    }

    const token& parser::peek(u64 offset) const
    {
        if (!valid_peek(offset))
            throw std::runtime_error("invalid peek offset. possibly malformed statement");

        return mTokens[mCurrent + offset];
    }

    const token& parser::advance()
    {
        return mTokens[mCurrent++];
    }

    bool parser::match(token_kind kind)
    {
        if (peek().kind != kind)
            return false;

        advance();
        return true;
    }
}