#include "Parser.h"
#include <iostream>
namespace NajaLang
{
	std::unordered_map<TokenType,PrefixFn> Parser::m_PrefixFunctions =
		{
			{TOKEN_IDENTIFIER, &Parser::ParseIdentifierExpr},
			{TOKEN_NUMBER, &Parser::ParseNumExpr},
			{TOKEN_STRING, &Parser::ParseStrExpr},
	};

	std::unordered_map<TokenType,InfixFn> Parser::m_InfixFunctions{

	};

	Parser::Parser()
	{
	}
	Parser::~Parser()
	{
	}

	UniqueRef<Stmt> Parser::Parse(const std::vector<Token> &tokens)
	{
		ResetStatus();
		m_Tokens = tokens;

		return ParseAstStmts();
	}

	void Parser::ResetStatus()
	{
		m_CurPos = 0;
		std::vector<Token>().swap(m_Tokens);

		m_Stmts.reset();
		m_Stmts = CreateUniqueRef<AstStmts>();
	}

	UniqueRef<Stmt> Parser::ParseAstStmts()
	{
		auto result = CreateUniqueRef<AstStmts>();

		while (!IsMatchCurToken(TOKEN_EOF))
			result->stmts.emplace_back(ParseStmt());
		return result;
	}

	UniqueRef<Stmt> Parser::ParseStmt()
	{
		return ParseExprStmt();
	}

	UniqueRef<Stmt> Parser::ParseExprStmt()
	{
		auto exprStmt = CreateUniqueRef<ExprStmt>(ParseExpr());
		Consume(TOKEN_SEMICOLON, "Expect ';' after expr stmt.");
		return exprStmt;
	}

	UniqueRef<Expr> Parser::ParseExpr()
	{
		if (m_PrefixFunctions.find(GetCurToken().type) == m_PrefixFunctions.end())
		{
			std::cout << "no prefix definition for:" << GetCurToken().literal << std::endl;
			return nullptr;
		}
		auto prefixFn = m_PrefixFunctions[GetCurToken().type];

		auto leftExpr = (this->*prefixFn)();

		return leftExpr;
	}

	UniqueRef<Expr> Parser::ParseIdentifierExpr()
	{
		return CreateUniqueRef<IdentifierExpr>(Consume(TOKEN_IDENTIFIER,"Expect a identifier.").literal);
	}

	UniqueRef<Expr> Parser::ParseNumExpr()
	{
		std::string numLiteral = Consume(TOKEN_NUMBER, "Expexct a number literal.").literal;

		if (numLiteral.find('.') != std::string::npos)
			return CreateUniqueRef<FloatNumExpr>(std::stod(numLiteral));
		else
			return CreateUniqueRef<IntNumExpr>(std::stoll(numLiteral));
	}

	UniqueRef<Expr> Parser::ParseStrExpr()
	{
		return CreateUniqueRef<StrExpr>(Consume(TOKEN_STRING, "Expect a string literal.").literal);
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
		std::cout << "[line " << GetCurToken().line << "]:" << errMsg << std::endl;
		return m_Tokens.back();
	}

	bool Parser::IsAtEnd()
	{
		return m_CurPos >= m_Tokens.size();
	}
}