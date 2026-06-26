#pragma once
#include <string_view>
#include "token.hpp"

namespace kit
{
    class kit_lexer
    {
    public:
        explicit kit_lexer(std::string_view sourceCode);

    public:
        token next();

    private:
        [[nodiscard]] char peek() const;
        char advance();

    private:
        void skip_comment();
        void skip_spaces();

    private:
        token lex_identifier();
        token lex_register();
        token lex_integer();
        token lex_pointer();
        token lex_section();

    private:
        std::string_view mSourceCode;
        u64 mPosition = 0;

        location mLocation;
    };
}