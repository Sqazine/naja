#include "Parser.h"
#include <iostream>
namespace NajaLang
{

	static NullExpr *nullExpr = new NullExpr();
	static TrueExpr *trueExpr = new TrueExpr();
	static FalseExpr *falseExpr = new FalseExpr();

	std::unordered_map<TokenType, PrefixFn> Parser::m_PrefixFunctions =
		{
			{TOKEN_IDENTIFIER, &Parser::ParseIdentifierExpr},
			{TOKEN_NUMBER, &Parser::ParseNumExpr},
			{TOKEN_STRING, &Parser::ParseStrExpr},
			{TOKEN_NULL, &Parser::ParseNullExpr},
			{TOKEN_TRUE, &Parser::ParseTrueExpr},
			{TOKEN_FALSE, &Parser::ParseFalseExpr},
			{TOKEN_MINUS, &Parser::ParsePrefixExpr},
			{TOKEN_BANG, &Parser::ParsePrefixExpr},
			{TOKEN_TILDE, &Parser::ParsePrefixExpr},
			{TOKEN_AMPERSAND, &Parser::ParsePrefixExpr},
			{TOKEN_PLUS_PLUS, &Parser::ParsePrefixExpr},
			{TOKEN_MINUS_MINUS, &Parser::ParsePrefixExpr}};

	std::unordered_map<TokenType, InfixFn> Parser::m_InfixFunctions{

	};

	std::unordered_map<TokenType, PostfixFn> Parser::m_PostfixFunctions{

	};

	Parser::Parser()
		: m_Stmts(nullptr)
	{
	}
	Parser::~Parser()
	{
	}

	Stmt *Parser::Parse(const std::vector<Token> &tokens)
	{
		ResetStatus();
		m_Tokens = tokens;

		return ParseAstStmts();
	}

	void Parser::ResetStatus()
	{
		m_CurPos = 0;
		std::vector<Token>().swap(m_Tokens);

		if (m_Stmts != nullptr)
		{
			delete m_Stmts;
			m_Stmts = nullptr;
		}
		m_Stmts = new AstStmts();
	}

	Stmt *Parser::ParseAstStmts()
	{
		while (!IsMatchCurToken(TOKEN_EOF))
			m_Stmts->stmts.emplace_back(ParseStmt());
		return m_Stmts;
	}

	Stmt *Parser::ParseStmt()
	{
		if (IsMatchCurToken(TOKEN_VAR))
			return ParseVarStmt();
		else if (IsMatchCurToken(TOKEN_RETURN))
			return ParseReturnStmt();
		else if (IsMatchCurToken(TOKEN_IF))
			return ParseIfStmt();
		else if (IsMatchCurToken(TOKEN_LEFT_BRACE))
			return ParseScopeStmt();
		else
			return ParseExprStmt();
	}

	Stmt *Parser::ParseExprStmt()
	{
		auto exprStmt = new ExprStmt(ParseExpr());
		Consume(TOKEN_SEMICOLON, "Expect ';' after expr stmt.");
		return exprStmt;
	}

	Stmt *Parser::ParseVarStmt()
	{
		Consume(TOKEN_VAR, "Expect 'var' key word");
		auto varStmt = new VarStmt();

		//the first variable
		auto identifier = new IdentifierExpr(Consume(TOKEN_IDENTIFIER, "Expect valid identifier").literal);
		Expr *value = nullExpr;
		if (IsMatchCurTokenAndStepOnce(TOKEN_EQUAL))
			value = ParseExpr();
		varStmt->variables[identifier] = value;

		//other variable
		while (IsMatchCurTokenAndStepOnce(TOKEN_COMMA))
		{
			identifier = new IdentifierExpr(Consume(TOKEN_IDENTIFIER, "Expect valid identifier").literal);
			Expr *value = nullExpr;
			if (IsMatchCurTokenAndStepOnce(TOKEN_EQUAL))
				value = ParseExpr();
			varStmt->variables[identifier] = value;
		}

		Consume(TOKEN_SEMICOLON, "Expect ';' after var stmt.");

		return varStmt;
	}

	Stmt *Parser::ParseReturnStmt()
	{
		Consume(TOKEN_RETURN, "Expect 'return' key word.");
		auto returnStmt = new ReturnStmt(ParseExpr());
		Consume(TOKEN_SEMICOLON, "Expect ';' after return stmt");
		return returnStmt;
	}

	Stmt *Parser::ParseIfStmt()
	{
		Consume(TOKEN_IF, "Expect 'if' key word.");
		Consume(TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");

		auto ifStmt = new IfStmt();

		ifStmt->condition = ParseExpr();

		Consume(TOKEN_RIGHT_PAREN, "Expect ')' after if condition");

		ifStmt->thenBranch = ParseStmt();

		if (IsMatchCurTokenAndStepOnce(TOKEN_ELSE))
			ifStmt->elseBranch = ParseStmt();

		return ifStmt;
	}

	Stmt *Parser::ParseScopeStmt()
	{
		Consume(TOKEN_LEFT_BRACE, "Expect '{'.");
		auto scopeStmt = new ScopeStmt();
		while (!IsMatchCurToken(TOKEN_RIGHT_BRACE))
			scopeStmt->stmts.emplace_back(ParseStmt());
		Consume(TOKEN_RIGHT_BRACE, "Expect '}'.");
		return scopeStmt;
	}

	Expr *Parser::ParseExpr(Precedence precedence)
	{
		if (m_PrefixFunctions.find(GetCurToken().type) == m_PrefixFunctions.end())
		{
			std::cout << "no prefix definition for:" << GetCurTokenAndStepOnce().literal << std::endl;
			return nullExpr;
		}
		auto prefixFn = m_PrefixFunctions[GetCurToken().type];

		auto leftExpr = (this->*prefixFn)();

		return leftExpr;
	}

	Expr *Parser::ParseIdentifierExpr()
	{
		return new IdentifierExpr(Consume(TOKEN_IDENTIFIER, "Expect a identifier.").literal);
	}

	Expr *Parser::ParseNumExpr()
	{
		std::string numLiteral = Consume(TOKEN_NUMBER, "Expexct a number literal.").literal;

		if (numLiteral.find('.') != std::string::npos)
			return new FloatNumExpr(std::stod(numLiteral));
		else
			return new IntNumExpr(std::stoll(numLiteral));
	}

	Expr *Parser::ParseStrExpr()
	{
		return new StrExpr(Consume(TOKEN_STRING, "Expect a string literal.").literal);
	}

	Expr *Parser::ParseNullExpr()
	{
		Consume(TOKEN_NULL, "Expect 'null' keyword");
		return nullExpr;
	}
	Expr *Parser::ParseTrueExpr()
	{
		Consume(TOKEN_TRUE, "Expect 'true' keyword");
		return trueExpr;
	}
	Expr *Parser::ParseFalseExpr()
	{
		Consume(TOKEN_FALSE, "Expect 'false' keyword");
		return falseExpr;
	}

	Expr *Parser::ParsePrefixExpr()
	{
		auto prefixExpr = new PrefixExpr();
		prefixExpr->op = GetCurTokenAndStepOnce().literal;
		prefixExpr->expr = ParseExpr();
		return prefixExpr;
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

	Token Parser::Consume(TokenType type, std::string errMsg)
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