#pragma once
#include <cstdint>
#include <string>

#include <ostream>
namespace NajaLang
{

    enum TokenType
    {
        TOKEN_NUMBER,
        TOKEN_STRING,
        TOKEN_IDENTIFIER,

        TOKEN_DOT,           // .
        TOKEN_COMMA,         // ,
        TOKEN_COLON,         // :
        TOKEN_SEMICOLON,     // ;
        TOKEN_LEFT_BRACKET,  // [
        TOKEN_RIGHT_BRACKET, // ]
        TOKEN_LEFT_BRACE,    // {
        TOKEN_RIGHT_BRACE,   // }
        TOKEN_LEFT_PAREN,    // (
        TOKEN_RIGHT_PAREN,   // )
        TOKEN_QUESTION,      // ?
        TOKEN_PLUS,          // +
        TOKEN_MINUS,         // -
        TOKEN_ASTERISK,      // *
        TOKEN_SLASH,         // /
        TOKEN_PERCENT,       // %
        TOKEN_BANG,          // !
        TOKEN_AMPERSAND,     // &
        TOKEN_VBAR,          // |
        TOKEN_CARET,         // ^
        TOKEN_TILDE,         // ~
        TOKEN_EQUAL,         // =
        TOKEN_LESS,          // <
        TOKEN_GREATER,       // >

        TOKEN_EQUAL_EQUAL,           // ==
        TOKEN_PLUS_EQUAL,            // +=
        TOKEN_MINUS_EQUAL,           // -=
        TOKEN_ASTERISK_EQUAL,        // *=
        TOKEN_PERCENT_EQUAL,         // %=
        TOKEN_SLASH_EQUAL,           // /=
        TOKEN_LESS_EQUAL,            // <=
        TOKEN_GREATER_EQUAL,         // >=
        TOKEN_AMPERSAND_EQUAL,       // &=
        TOKEN_VBAR_EQUAL,            // |=
        TOKEN_TILDE_EQUAL,           // ~=
        TOKEN_CARET_EQUAL,           // ^=
        TOKEN_BANG_EQUAL,            // !=
        TOKEN_LESS_LESS_EQUAL,       // <<=
        TOKEN_GREATER_GREATER_EQUAL, // >>=
        TOKEN_PLUS_PLUS,             // ++
        TOKEN_MINUS_MINUS,           // --
        TOKEN_AMPERSAND_AMPERSAND,   // &&
        TOKEN_VBAR_VBAR,             // ||
        TOKEN_LESS_LESS,             // <<
        TOKEN_GREATER_GREATER,       // >>

        TOKEN_VAR,       // var
        TOKEN_IF,        // if
        TOKEN_ELSE,      // else
        TOKEN_TRUE,      // true
        TOKEN_FALSE,     // false
        TOKEN_NULL,      // null
        TOKEN_WHILE,     // while
        TOKEN_FOR,       // for
        TOKEN_BREAK,     // break
        TOKEN_CONTINUE,  // continue
        TOKEN_FUNCTION,  // function
        TOKEN_CLASS,     // class
        TOKEN_PUBLIC,    // public
        TOKEN_PROTECTED, // protected
        TOKEN_PRIVATE,   // private
        TOKEN_THIS,      // this
        TOKEN_BASE,      // base
        TOKEN_NEW,// new
        TOKEN_RETURN,    // return

        TOKEN_UNDEFINED,
        TOKEN_EOF
    };

    struct Token
    {
        Token(TokenType type, std::string literal, uint64_t line) : type(type), literal(literal), line(line) {}

        TokenType type;
        std::string literal;
        uint64_t line;
    };

    inline std::ostream &operator<<(std::ostream &stream, const Token &token)
    {
        return stream << token.type << ",'" << token.literal << "'," << token.line;
    }
}