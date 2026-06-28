#include <iostream>

#include "utils/file.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "assembler/assembler.hpp"

#include "platform/platform.hpp"

[[maybe_unused]]
static void debug_lexer(const std::vector<kit::token>& tokens)
{
    for (const auto& token : tokens)
    {
        if (!token.lexeme.empty())
            std::cout
                << token.lexeme << ' '
                << get_token_kind_text(token.kind) << ' '
                << '(' << token.tokenLocation.line << ", " << token.tokenLocation.column << ')' 
                << '\n';
        else
            std::cout
                << std::uppercase << get_token_kind_text(token.kind) << ' '
                << '(' << token.tokenLocation.line << ", " << token.tokenLocation.column << ')' 
                << '\n';
    }
}

[[maybe_unused]]
static void debug_parser(const std::vector<kit::instruction>& instructions)
{
    for (const auto& instruction : instructions)
    {
        std::cout << std::uppercase << kit::get_opcode_text(instruction.code) << ": [ ";
        
        for (const auto& operand : instruction.operands)
        {
            if (operand.type == kit::operand::kind::immediate)
                std::cout << operand.immediate;
            else
                std::cout << kit::get_register_text(operand.register_);
            std::cout << ' ';
        }

        std::cout << "]\n";
    }
}

[[maybe_unused]]
static void debug_compiled_code(const std::vector<kit::u8>& code)
{
    for (const auto& c : code)
        std::cout << std::hex << (int)c << ' ';
    
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "usage: program <file>\n";
        return 1;
    }

    std::filesystem::path filepath = argv[1];
    std::string source = kit::file_read_contents(filepath);

    // Lexer:
    kit::kit_lexer lexer { source };
    std::vector<kit::token> tokens;
    while (true)
    {
        auto token = lexer.next();
        tokens.push_back(token);

        if (token.kind == kit::token_kind::eof)
            break;
    }

    // Parser:
    kit::parser parser(tokens);
    parser.parse();

    // Assembler:
    kit::assembler assembler;
    assembler.assemble(parser.get_statements());

    // Write:
    std::ofstream file("main", std::ios::binary);
    kit::platform::write_executable(file, assembler.get_segments(), std::move(assembler.get_realloactions()) );

    return 0;
}