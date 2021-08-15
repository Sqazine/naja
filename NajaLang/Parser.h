#pragma once
#include "Token.h"
#include <vector>
#include "Ast.h"
namespace NajaLang
{
    class Parser
    {
    public:
        Parser();
        ~Parser();

    private:
        std::vector<Token> m_Tokens;
    };
}