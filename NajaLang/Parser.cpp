#include "Parser.h"
#include <iostream>
namespace NajaLang
{
	Parser::Parser()
	{
	}
	Parser::~Parser()
	{
	}

	UniqueRef<Expr> Parser::Parse(const std::vector<Token>& tokens)
	{
		ResetStatus();
		m_Tokens = tokens;

		return ParseNumExpr();
	}

	void Parser::ResetStatus()
	{
		m_CurPos = 0;
		std::vector<Token>().swap(m_Tokens);

	/*	m_Stmts.reset();
		m_Stmts = CreateUniqueRef<AstStmts>();*/
	}

	UniqueRef<Expr> Parser::ParseNumExpr()
	{
		std::string numLiteral = GetCurToken().literal;

		if (numLiteral.find('.') != std::string::npos)
			return CreateUniqueRef<FloatNumExpr>(std::stod(numLiteral));
		else return CreateUniqueRef<IntNumExpr>(std::stoll(numLiteral));
	}

	Token Parser::GetCurToken()
	{
		if (!IsAtEnd())
			return m_Tokens[m_CurPos];
		return m_Tokens.back();
	}
	Token Parser::GetCurTokenAndStepOnce()
	{
		if (!IsAtEnd())
			return m_Tokens[m_CurPos++];
		return m_Tokens.back();
	}

	Token Parser::GetNextToken()
	{
		if (m_CurPos + 1 < m_Tokens.size())
			return m_Tokens[m_CurPos + 1];
		return m_Tokens.back();
	}
	Token Parser::GetNextTokenAndStepOnce()
	{
		if (m_CurPos + 1 < m_Tokens.size())
			return m_Tokens[++m_CurPos];
		return m_Tokens.back();
	}

	Token Parser::GetPreToken()
	{
		return m_Tokens[m_CurPos - 1];
	}

	bool Parser::IsMatchCurToken(TokenType type)
	{
		return GetCurToken().type == type;
	}

	bool Parser::IsMatchCurTokenAndStepOnce(TokenType type)
	{
		if (IsMatchCurToken(type))
		{
			m_CurPos++;
			return true;
		}
		return false;
	}

	bool Parser::IsMatchNextToken(TokenType type)
	{
		return GetNextToken().type == type;
	}

	bool Parser::IsMatchNextTokenAndStepOnce(TokenType type)
	{
		if (IsMatchNextToken(type))
		{
			m_CurPos++;
			return true;
		}
		return false;
	}

	bool Parser::IsMatchPreToken(TokenType type)
	{
		return GetPreToken().type == type;
	}


	Token Parser::Consume(TokenType type, std::string_view errMsg)
	{
		if (IsMatchCurToken(type))
			return GetCurTokenAndStepOnce();
		std::cout << errMsg << std::endl;
		return m_Tokens.back();
	}

	bool Parser::IsAtEnd()
	{
		return m_CurPos >= m_Tokens.size();
	}
}