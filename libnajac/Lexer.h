#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <string_view>
#include "Token.h"
namespace NajaC
{
    class Lexer
    {
    public:
        Lexer();
        ~Lexer();

       const std::vector<Token>& ScanTokens(std::string_view src);
    private:

        void ScanToken();

        void ResetStatus();

        bool IsMatchCurChar(char c);
        bool IsMatchCurCharAndStepOnce(char c);

        char GetNextCharAndStepOnce();
        char GetNextChar();
        char GetCurCharAndStepOnce();
        char GetCurChar();

        void AddToken(TokenType type);
        void AddToken(TokenType type,std::string_view literal);

        bool IsAtEnd();

        bool IsNumber(char c);
        bool IsLetter(char c);
        bool IsLetterOrNumber(char c);

        void Number();
        void Identifier();
        void String();

        uint64_t m_StartPos;
        uint64_t m_CurPos;
        uint64_t m_Line;
        std::string m_Source;
        std::vector<Token> m_Tokens;
    };
}