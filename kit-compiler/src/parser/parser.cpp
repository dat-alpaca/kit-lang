#include <stdexcept>

#include "parser.hpp"
#include "opcode_table.hpp"
#include "lexer/token.hpp"
#include "parser/instruction.hpp"
#include "parser/opcode.hpp"
#include "parser/register.hpp"
#include "utils/string_utils.hpp"

namespace kit
{
    kit::parser::parser(std::span<const token> tokens)
        : mTokens(tokens)
    {

    }

    std::vector<instruction> parser::parse()
    {
        std::vector<instruction> instructions;
        instructions.reserve(mTokens.size());

        while (peek().kind != token_kind::eof)
            instructions.push_back(parse_instruction());

        return instructions;
    }
}

namespace kit
{
    instruction parser::parse_instruction()
    {
        instruction instruction = { .code = parse_opcode() };
        
        if (!opcode_has_operands(instruction.code))
            return instruction;

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
            case token_kind::token_register:
            {
                register_k reg;
                if (current.lexeme == "%AX")
                    reg = register_k::ax;
                else
                    throw std::runtime_error("failed to parse operands: unknown register");

                return {
                    .type = operand::kind::register_,
                    .register_ = reg
                };
            }

            case token_kind::integer:
            {
                return {
                    .type = operand::kind::immediate,
                    .immediate = std::stoull(std::string(current.lexeme))
                };
            }

            default:
                throw std::runtime_error("failed to parse operands: invalid token kind");
        }
    }
}

namespace kit
{
    const token& parser::peek() const
    {
        return mTokens[mCurrent];
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