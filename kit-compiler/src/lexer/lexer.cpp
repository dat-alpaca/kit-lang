#include <cctype>
#include <stdexcept>

#include "lexer/lexer.hpp"
#include "constants.hpp"
#include "token.hpp"

static bool is_possible_identifier(char character)
{
    bool isPossible = false;
    isPossible |= std::isalpha(static_cast<unsigned char>(character));
        
    for (auto& possibleFirst : kit::gValidIdentifierFirstCharacters)
        isPossible |= (possibleFirst == character);

    return isPossible;
}

static bool is_valid_identifier_character(char character)
{
    return character == '_' || std::isalnum(static_cast<unsigned char>(character));
}

namespace kit
{
    kit_lexer::kit_lexer(std::string_view sourceCode)
        : mSourceCode(sourceCode)
    {
        /* Blank */
    }

    token kit_lexer::next()
    {
        skip_spaces();

        location currentLocation = mLocation;
        char character = peek();

        if (character == '\0')
            return { token_kind::eof, {}, currentLocation };

        if (character == '\n')
        {
            advance();
            return { token_kind::newline, {}, currentLocation };
        }

        if (is_possible_identifier(character))
            return lex_identifier();

        if (character == gRegisterSymbol)
            return lex_register();

        if (std::isdigit(character))
            return lex_integer();

        if (character == ',')
        {
            advance();
            return { token_kind::comma, {}, currentLocation };
        }

        throw std::runtime_error("attemped to lex an unexpected character");
    }
}

namespace kit
{
    char kit_lexer::peek() const
    {
        if (mPosition >= mSourceCode.size())
            return '\0';

        return mSourceCode[mPosition];
    }

    char kit_lexer::advance()
    {
        char character = peek();

        if (character == '\n')
        {
            ++mLocation.line;
            mLocation.column = 1;
        }
        else
            ++mLocation.column;

        ++mPosition;

        return character;
    }

    void kit_lexer::skip_comment()
    {
        while (true)
        {
            char character = peek();
            if (character == '\n' || character == '\0')
                break;

            advance();
        }
    }

    void kit_lexer::skip_spaces()
    {
        while (true)
        {
            char character = peek();

            if (character == gCommentSymbol)
            {
                skip_comment();
                continue;
            }

            if (std::isspace(character))
            {
                advance();
                continue;
            }

            break;
        }
    }
}

namespace kit
{
    token kit_lexer::lex_identifier()
    {
        u64 startingPosition = mPosition;
        location currentLocation = mLocation;

        while (true)
        {
            char character = peek();
            if (!is_valid_identifier_character(character))
                break;

            advance();
        }

        u64 finalPosition = mPosition - startingPosition; 
        return { token_kind::identifier, mSourceCode.substr(startingPosition, finalPosition), currentLocation };
    }

    token kit_lexer::lex_register()
    {
        u64 startingPosition = mPosition;
        location currentLocation = mLocation;

        advance(); // skips the %

        while (std::isalnum(static_cast<unsigned char>(peek())))
            advance();
    
        u64 finalPosition = mPosition - startingPosition;
        return { token_kind::token_register, mSourceCode.substr(startingPosition, finalPosition), currentLocation };
    }
    
    token kit_lexer::lex_integer()
    {
        u64 startingPosition = mPosition;
        location currentLocation = mLocation;

        while (std::isdigit(static_cast<unsigned char>(peek())))
            advance();

        u64 finalPosition = mPosition - startingPosition;
        return { token_kind::integer, mSourceCode.substr(startingPosition, finalPosition), currentLocation };
    }
}