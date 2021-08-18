#include "Parser.h"
#include <iostream>
namespace NajaLang
{

	const SharedRef<NullExpr> nullExpr = CreateShared<NullExpr>();
	const SharedRef<TrueExpr> trueExpr = CreateShared<TrueExpr>();
	const SharedRef<FalseExpr> falseExpr = CreateShared<FalseExpr>();

	std::unordered_map<TokenType, PrefixFn> Parser::m_PrefixFunctions =
		{
			{TOKEN_IDENTIFIER, &Parser::ParseIdentifierExpr},
			{TOKEN_NUMBER, &Parser::ParseNumExpr},
			{TOKEN_STRING, &Parser::ParseStrExpr},
			{TOKEN_NULL, &Parser::ParseNullExpr},
			{TOKEN_TRUE, &Parser::ParseTrueExpr},
			{TOKEN_FALSE, &Parser::ParseFalseExpr}};

	std::unordered_map<TokenType, InfixFn> Parser::m_InfixFunctions{

	};

	Parser::Parser()
	{
	}
	Parser::~Parser()
	{
	}

	SharedRef<Stmt> Parser::Parse(const std::vector<Token> &tokens)
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
		m_Stmts = CreateShared<AstStmts>();
	}

	SharedRef<Stmt> Parser::ParseAstStmts()
	{
		auto result = CreateShared<AstStmts>();

		while (!IsMatchCurToken(TOKEN_EOF))
			result->stmts.emplace_back(ParseStmt());
		return result;
	}

	SharedRef<Stmt> Parser::ParseStmt()
	{
		if (IsMatchCurToken(TOKEN_VAR))
			return ParseVarStmt();
		else
			return ParseExprStmt();
	}

	SharedRef<Stmt> Parser::ParseExprStmt()
	{
		auto exprStmt = CreateShared<ExprStmt>(ParseExpr());
		Consume(TOKEN_SEMICOLON, "Expect ';' after expr stmt.");
		return exprStmt;
	}

	SharedRef<Stmt> Parser::ParseVarStmt()
	{
		Consume(TOKEN_VAR, "Expect 'var' key word");
		auto varStmt = CreateShared<VarStmt>();

		//the first variable
		auto identifier = CreateShared<IdentifierExpr>(Consume(TOKEN_IDENTIFIER, "Expect valid identifier").literal);
		SharedRef<Expr> value = nullExpr;
		if (IsMatchCurTokenAndStepOnce(TOKEN_EQUAL))
			value = ParseExpr();
		varStmt->variables[identifier] = value;

		//other variable
		while (IsMatchCurTokenAndStepOnce(TOKEN_COMMA))
		{
			identifier = CreateShared<IdentifierExpr>(Consume(TOKEN_IDENTIFIER, "Expect valid identifier").literal);
			SharedRef<Expr> value = nullExpr;
			if (IsMatchCurTokenAndStepOnce(TOKEN_EQUAL))
				value = ParseExpr();
			varStmt->variables[identifier] = value;
		}

		Consume(TOKEN_SEMICOLON, "Expect ';' after var stmt.");

		return varStmt;
	}

	SharedRef<Expr> Parser::ParseExpr()
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

	SharedRef<Expr> Parser::ParseIdentifierExpr()
	{
		return CreateShared<IdentifierExpr>(Consume(TOKEN_IDENTIFIER, "Expect a identifier.").literal);
	}

	SharedRef<Expr> Parser::ParseNumExpr()
	{
		std::string numLiteral = Consume(TOKEN_NUMBER, "Expexct a number literal.").literal;

		if (numLiteral.find('.') != std::string::npos)
			return CreateShared<FloatNumExpr>(std::stod(numLiteral));
		else
			return CreateShared<IntNumExpr>(std::stoll(numLiteral));
	}

	SharedRef<Expr> Parser::ParseStrExpr()
	{
		return CreateShared<StrExpr>(Consume(TOKEN_STRING, "Expect a string literal.").literal);
	}

	SharedRef<Expr> Parser::ParseNullExpr()
	{
		Consume(TOKEN_NULL, "Expect 'null' keyword");
		return nullExpr;
	}
	SharedRef<Expr> Parser::ParseTrueExpr()
	{
		Consume(TOKEN_TRUE, "Expect 'true' keyword");
		return trueExpr;
	}
	SharedRef<Expr> Parser::ParseFalseExpr()
	{
		Consume(TOKEN_FALSE, "Expect 'false' keyword");
		return falseExpr;
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
		if (m_CurPos + 1 < (int32_t)m_Tokens.size())
			return m_Tokens[m_CurPos + 1];
		return m_Tokens.back();
	}
	Token Parser::GetNextTokenAndStepOnce()
	{
		if (m_CurPos + 1 < (int32_t)m_Tokens.size())
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
		return m_CurPos >= (int32_t)m_Tokens.size();
	}
}