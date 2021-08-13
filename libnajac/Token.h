#pragma once

namespace NajaC
{

    enum class TokenType
    {
        TOKEN_NUMBER,
        TOKEN_STRING,
        TOKEN_IDENTIFIER,
        TOKEN_DOT,           // .
        TOKEN_COMMA,         // ,
        TOKEN_COLON,         // :
        TOKEN_SEMICOLON,     // ;
        TOKEN_NUMBER_SIGN,   // #
        TOKEN_LEFT_BRACKET,  // [
        TOKEN_RIGHT_BRACKET, // ]
        TOKEN_LEFT_BRACE,    // {
        TOKEN_RIGHT_BRACE,   // }
        TOKEN_LEFT_PAREN,    // (
        TOKEN_RIGHT_PAREN,   // )
        TOKEN_PLUS,          // +
        TOKEN_MINUS,         // -
        TOKEN_STAR,          // *
        TOKEN_SLASH,         // /
        TOKEN_QUESTION,      // ?
        TOKEN_BANG,          // !
        TOKEN_AMPERSAND,     // &
        TOKEN_VERTICAL_BAR,  // |
        TOKEN_CARET,         // ^
        TOKEN_TILDE,         // ~
        TOKEN_AT,            // @
    };

}