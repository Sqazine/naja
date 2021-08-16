#pragma once
#include "Token.h"
#include <vector>
#include "Ast.h"
#include "Utils.h"
namespace NajaLang
{
	class Parser
	{
	public:
		Parser();
		~Parser();

		UniqueRef<Expr> Parse(const std::vector<Token>& tokens);

	private:
		void ResetStatus();

		UniqueRef<Expr> ParseNumExpr();

		Token GetCurToken();
		Token GetCurTokenAndStepOnce();

		Token GetNextToken();
		Token GetNextTokenAndStepOnce();

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


		Token Consume(TokenType type, std::string_view errMsg);

		template<typename... T>
		Token Consume(T... type, std::string_view errMsg);

		bool IsAtEnd();

		int64_t m_CurPos;
		//UniqueRef<AstStmts> m_Stmts;
		std::vector<Token> m_Tokens;
	};
	template<typename ...T>
	inline bool Parser::IsMatchCurToken(T ...type)
	{
		assert((...&&std::is_same_v<T, TokenType>));

		if ((... || IsMatchCurToken(type)))
			return true;
		return false;
	}
	template<typename ...T>
	inline bool Parser::IsMatchCurTokenAndStepOnce(T ...type)
	{
		assert((...&&std::is_same_v<T, TokenType>));

		if ((... || IsMatchCurTokenAndStepOnce(type)))
			return true;
		return false;
	}
	template<typename ...T>
	inline bool Parser::IsMatchNextToken(T ...type)
	{
		assert((...&&std::is_same_v<T, TokenType>));

		if ((... || IsMatchNextToken(type)))
			return true;
		return false;
	}
	template<typename ...T>
	inline bool Parser::IsMatchNextTokenAndStepOnce(T ...type)
	{
		assert((...&&std::is_same_v<T, TokenType>));

		if ((... || IsMatchNextTokenAndStepOnce(type)))
			return true;
		return false;
	}
	template<typename ...T>
	inline bool Parser::IsMatchPreToken(T ...type)
	{
		assert((...&&std::is_same_v<T, TokenType>));

		if ((... || IsMatchPreToken(type)))
			return true;
		return false;
	}
	template<typename ...T>
	inline Token Parser::Consume(T ...type, std::string_view errMsg)
	{
		assert((...&&std::is_same_v<T, TokenType>));

		return Consume(type, errMsg);
	}
}