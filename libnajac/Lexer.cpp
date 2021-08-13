#include "Lexer.h"

namespace NajaC
{

    const std::vector<Token> &Lexer::ScanTokens(std::string_view src)
    {
        ResetStatus();
        m_Source = src;
        while (!IsAtEnd())
        {
            m_StartPos = m_CurPos;
            ScanToken();
        }

        AddToken(Token(TOKEN_EOF, "", m_Line));

        return m_Tokens;
    }
    void Lexer::ScanToken()
    {
    }

    void Lexer::ResetStatus()
    {
        m_StartPos = m_CurPos = m_Line = 0;
        std::vector<Token>().swap(m_Tokens);
    }

    char Lexer::GetNextCharAndStepOnce()
    {
        
    }
    char Lexer::GetNextChar()
    {
    }
    char Lexer::GetCurCharAndStepOnce()
    {
        if(!IsAtEnd())
            return m_Source[m_CurPos++];
        return '\0';
    }

    char Lexer::GetCurChar()
    {
         if(!IsAtEnd())
            return m_Source[m_CurPos];
        return '\0';
    }

    void Lexer::AddToken(Token token)
    {
        m_Tokens.emplace_back(token);
    }

    bool Lexer::IsAtEnd()
    {
        return m_CurPos >= m_Source.length();
    }

    bool Lexer::IsNumber(char c)
    {
        return c >= '0' && c <= '9';
    }
    bool Lexer::IsLetter(char c)
    {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
    }
    bool Lexer::IsLetterOrNumber(char c)
    {
        return IsLetter(c) || IsNumber(c);
    }
}