#pragma once
#include <vector>
#include <unordered_map>
#include "Token.h"
#include "Ast.h"
namespace NajaLang
{

	enum Precedence
	{
		LOWEST,
		ASSIGN,		 // = += -= *= /= %= &= ^= |= <<= >>=
		TERNARY,	 // ? :
		LOGIC_OR,	 // ||
		LOGIC_AND,	 // &&
		BIT_OR,		 // |
		BIT_XOR,	 // ^
		BIT_AND,	 // &
		EQUAL,		 // == !=
		COMPARE,	 // < <= > >=
		BIT_SHIFT,	 // << >>
		ADD_PLUS,	 // + -
		MUL_DIV_MOD, // * / %

		// PREFIX,		 // ++ -- ! ~ &
		POSTFIX,	 // ++ --
	};

	class Parser;

	typedef Expr *(Parser::*PrefixFn)();
	typedef Expr *(Parser::*InfixFn)(Expr *);
	typedef Expr *(Parser::*PostfixFn)(Expr *);

	class Parser
	{
	public:
		Parser();
		~Parser();

		Stmt *Parse(const std::vector<Token> &tokens);

	private:
		void ResetStatus();

		Stmt *ParseAstStmts();
		Stmt *ParseStmt();
		Stmt *ParseExprStmt();
		Stmt *ParseVarStmt();
		Stmt *ParseReturnStmt();
		Stmt *ParseIfStmt();
		Stmt *ParseScopeStmt();
		Stmt *ParseWhileStmt();
		Stmt *ParseForStmt();
		Stmt *ParseBreakStmt();
		Stmt *ParseContinueStmt();
		Stmt *ParseFunctionStmt();

		Expr *ParseExpr(Precedence precedence = LOWEST);
		Expr *ParseIdentifierExpr();
		Expr *ParseNumExpr();
		Expr *ParseStrExpr();
		Expr *ParseNullExpr();
		Expr *ParseTrueExpr();
		Expr *ParseFalseExpr();
		Expr* ParseFunctionExpr();
		Expr* ParseArrayExpr();
		Expr* ParseTableExpr();
		Expr *ParsePrefixExpr();
		Expr *ParseInfixExpr(Expr *prefixExpr);
		Expr *ParsePostfixExpr(Expr *prefixExpr);
		Expr* ParseTernaryExpr(Expr* prefixExpr);

		Token GetCurToken();
		Token GetCurTokenAndStepOnce();
		Precedence GetCurTokenPrecedence();

		Token GetNextToken();
		Token GetNextTokenAndStepOnce();
		Precedence GetNextTokenPrecedence();

		Token GetPreToken();

		bool IsMatchCurToken(TokenType type);
		bool IsMatchCurTokenAndStepOnce(TokenType type);
		bool IsMatchNextToken(TokenType type);
		bool IsMatchNextTokenAndStepOnce(TokenType type);
		bool IsMatchPreToken(TokenType type);

		template <typename... T>
		bool IsMatchCurToken(T... type);
		template <typename... T>
		bool IsMatchCurTokenAndStepOnce(T... type);

		template <typename... T>
		bool IsMatchNextToken(T... type);
		template <typename... T>
		bool IsMatchNextTokenAndStepOnce(T... type);

		template <typename... T>
		bool IsMatchPreToken(T... type);

		Token Consume(TokenType type, std::string errMsg);

		template <typename... T>
		Token Consume(T... type, std::string errMsg);

		bool IsAtEnd();

		int64_t m_CurPos;
		AstStmts *m_Stmts;
		std::vector<Token> m_Tokens;

		static std::unordered_map<TokenType, PrefixFn> m_PrefixFunctions;
		static std::unordered_map<TokenType, InfixFn> m_InfixFunctions;
		static std::unordered_map<TokenType, PostfixFn> m_PostfixFunctions;
		static std::unordered_map<TokenType, Precedence> m_Precedence;
	};
	template <typename... T>
	inline bool Parser::IsMatchCurToken(T... type)
	{
		assert((... && std::is_same_v<T, TokenType>));

		if ((... || IsMatchCurToken(type)))
			return true;
		return false;
	}
	template <typename... T>
	inline bool Parser::IsMatchCurTokenAndStepOnce(T... type)
	{
		assert((... && std::is_same_v<T, TokenType>));

		if ((... || IsMatchCurTokenAndStepOnce(type)))
			return true;
		return false;
	}
	template <typename... T>
	inline bool Parser::IsMatchNextToken(T... type)
	{
		assert((... && std::is_same_v<T, TokenType>));

		if ((... || IsMatchNextToken(type)))
			return true;
		return false;
	}
	template <typename... T>
	inline bool Parser::IsMatchNextTokenAndStepOnce(T... type)
	{
		assert((... && std::is_same_v<T, TokenType>));

		if ((... || IsMatchNextTokenAndStepOnce(type)))
			return true;
		return false;
	}
	template <typename... T>
	inline bool Parser::IsMatchPreToken(T... type)
	{
		assert((... && std::is_same_v<T, TokenType>));

		if ((... || IsMatchPreToken(type)))
			return true;
		return false;
	}
	template <typename... T>
	inline Token Parser::Consume(T... type, std::string errMsg)
	{
		assert((... && std::is_same_v<T, TokenType>));

		return Consume(type, errMsg);
	}
}