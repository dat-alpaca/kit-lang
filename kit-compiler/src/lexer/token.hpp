#pragma once
#include <string_view>

#include "common.hpp"

namespace kit
{
    enum class token_kind
    {
        invalid = 0,
        identifier,
        
        token_register,
        integer,

        newline,
        comma,
        eof,
    };

    static inline constexpr const char* get_token_kind_text(token_kind kind)
    {
        switch (kind)
        {
            case token_kind::invalid: return "invalid"; 
            case token_kind::identifier: return "identifier"; 
            case token_kind::token_register: return "token_register"; 
            case token_kind::integer: return "integer"; 
            case token_kind::newline: return "newline"; 
            case token_kind::comma: return "comma"; 
            case token_kind::eof: return "eof";

            default:
                return "invalid";
        }
    }

    struct location
    {
        u32 line = 0;
        u32 column = 0;
    };

    struct token
    {
        token_kind kind = token_kind::invalid;
        std::string_view lexeme;
        location tokenLocation;
    };
}