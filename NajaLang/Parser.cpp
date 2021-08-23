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
			{TOKEN_MINUS_MINUS, &Parser::ParsePrefixExpr},
			{TOKEN_LEFT_PAREN, &Parser::ParseGroupExpr},
			{TOKEN_FUNCTION, &Parser::ParseFunctionExpr},
			{TOKEN_LEFT_BRACKET, &Parser::ParseArrayExpr},
			{TOKEN_LEFT_BRACE, &Parser::ParseTableExpr},

	};

	std::unordered_map<TokenType, InfixFn> Parser::m_InfixFunctions =
		{
			{TOKEN_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_PLUS_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_MINUS_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_ASTERISK_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_SLASH_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_PERCENT_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_AMPERSAND_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_CARET_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_VBAR_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_LESS_LESS_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_GREATER_GREATER_EQUAL, &Parser::ParseInfixExpr},

			{TOKEN_QUESTION, &Parser::ParseTernaryExpr},

			{TOKEN_VBAR_VBAR, &Parser::ParseInfixExpr},
			{TOKEN_AMPERSAND_AMPERSAND, &Parser::ParseInfixExpr},

			{TOKEN_VBAR, &Parser::ParseInfixExpr},
			{TOKEN_CARET, &Parser::ParseInfixExpr},
			{TOKEN_AMPERSAND, &Parser::ParseInfixExpr},

			{TOKEN_EQUAL_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_BANG_EQUAL, &Parser::ParseInfixExpr},

			{TOKEN_LESS, &Parser::ParseInfixExpr},
			{TOKEN_LESS_EQUAL, &Parser::ParseInfixExpr},
			{TOKEN_GREATER, &Parser::ParseInfixExpr},
			{TOKEN_GREATER_EQUAL, &Parser::ParseInfixExpr},

			{TOKEN_LESS_LESS, &Parser::ParseInfixExpr},
			{TOKEN_GREATER_GREATER, &Parser::ParseInfixExpr},

			{TOKEN_PLUS, &Parser::ParseInfixExpr},
			{TOKEN_MINUS, &Parser::ParseInfixExpr},
			{TOKEN_ASTERISK, &Parser::ParseInfixExpr},
			{TOKEN_SLASH, &Parser::ParseInfixExpr},
			{TOKEN_PERCENT, &Parser::ParseInfixExpr},
	};

	std::unordered_map<TokenType, PostfixFn> Parser::m_PostfixFunctions = {

		{TOKEN_PLUS_PLUS, &Parser::ParsePostfixExpr},
		{TOKEN_MINUS_MINUS, &Parser::ParsePostfixExpr},
	};

	std::unordered_map<TokenType, Precedence> Parser::m_Precedence = {
		{TOKEN_EQUAL, ASSIGN},
		{TOKEN_PLUS_EQUAL, ASSIGN},
		{TOKEN_MINUS_EQUAL, ASSIGN},
		{TOKEN_ASTERISK_EQUAL, ASSIGN},
		{TOKEN_SLASH_EQUAL, ASSIGN},
		{TOKEN_PERCENT_EQUAL, ASSIGN},
		{TOKEN_AMPERSAND_EQUAL, ASSIGN},
		{TOKEN_CARET_EQUAL, ASSIGN},
		{TOKEN_VBAR_EQUAL, ASSIGN},
		{TOKEN_LESS_LESS_EQUAL, ASSIGN},
		{TOKEN_GREATER_GREATER_EQUAL, ASSIGN},

		{TOKEN_QUESTION, TERNARY},
		{TOKEN_COLON, TERNARY},

		{TOKEN_VBAR_VBAR, LOGIC_OR},

		{TOKEN_AMPERSAND_AMPERSAND, LOGIC_AND},

		{TOKEN_VBAR, BIT_OR},

		{TOKEN_CARET, BIT_XOR},

		{TOKEN_AMPERSAND, BIT_AND},

		{TOKEN_EQUAL_EQUAL, EQUAL},
		{TOKEN_BANG_EQUAL, EQUAL},

		{TOKEN_LESS, COMPARE},
		{TOKEN_LESS_EQUAL, COMPARE},
		{TOKEN_GREATER, COMPARE},
		{TOKEN_GREATER_EQUAL, COMPARE},

		{TOKEN_LESS_LESS, BIT_SHIFT},
		{TOKEN_GREATER_GREATER, BIT_SHIFT},

		{TOKEN_PLUS, ADD_PLUS},
		{TOKEN_MINUS, ADD_PLUS},

		{TOKEN_ASTERISK, MUL_DIV_MOD},
		{TOKEN_SLASH, MUL_DIV_MOD},
		{TOKEN_PERCENT, MUL_DIV_MOD},

		{TOKEN_PLUS_PLUS, POSTFIX},
		{TOKEN_MINUS_MINUS, POSTFIX},
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
		else if (IsMatchCurToken(TOKEN_WHILE))
			return ParseWhileStmt();
		else if (IsMatchCurToken(TOKEN_FOR))
			return ParseForStmt();
		else if (IsMatchCurToken(TOKEN_BREAK))
			return ParseBreakStmt();
		else if (IsMatchCurToken(TOKEN_CONTINUE))
			return ParseContinueStmt();
		else if (IsMatchCurToken(TOKEN_FUNCTION))
			return ParseFunctionStmt();
		else if (IsMatchCurToken(TOKEN_CLASS))
			return ParseClassStmt();
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

	Stmt *Parser::ParseWhileStmt()
	{
		Consume(TOKEN_WHILE, "Expect 'while' keyword.");
		Consume(TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");

		auto whileStmt = new WhileStmt();

		whileStmt->condition = ParseExpr(LOWEST);

		Consume(TOKEN_RIGHT_PAREN, "Expect ')' after while stmt's condition.");

		whileStmt->stmt = ParseStmt();

		return whileStmt;
	}

	Stmt *Parser::ParseForStmt()
	{
		//for(i=0;j=0;i<10&&j<20;++i,++j)
		//{
		//	//for loop part...
		//}

		// |
		// v

		//{
		//	i=0;j=0;
		//	while(i<10&&j<10)
		//	{
		//		//for loop part...
		//		++i;++j;
		//	}
		//}
		Consume(TOKEN_FOR, "Expect 'for' keyword.");
		Consume(TOKEN_LEFT_PAREN, "Expect '(' after 'for'.");

		auto forStmt = new ScopeStmt();

		if (!IsMatchCurToken(TOKEN_SEMICOLON)) //has initializer
		{
			if (IsMatchCurToken(TOKEN_VAR)) // var decl initializer
				forStmt->stmts.emplace_back(ParseVarStmt());
			else //exprStmt
			{
				//the first exprStmt

				forStmt->stmts.emplace_back(new ExprStmt(ParseExpr()));
				while (IsMatchCurTokenAndStepOnce(TOKEN_COMMA))
					forStmt->stmts.emplace_back(new ExprStmt(ParseExpr()));
				Consume(TOKEN_SEMICOLON, "Expect ';' after initialize part of 'for' stmt");
			}
		}

		auto forCondition = ParseExpr();

		Consume(TOKEN_SEMICOLON, "Expect ';' after 'for' stmt condition part.");

		std::vector<ExprStmt *> independentVariables;
		if (!IsMatchCurToken(TOKEN_RIGHT_PAREN))
		{
			independentVariables.emplace_back(new ExprStmt(ParseExpr()));
			while (IsMatchCurTokenAndStepOnce(TOKEN_COMMA))
				independentVariables.emplace_back(new ExprStmt(ParseExpr()));
		}

		Consume(TOKEN_RIGHT_PAREN, "Expect ')' after 'for' stmt.");

		auto loopBody = new ScopeStmt();

		//for loop body is a scope stmt
		if (IsMatchCurToken(TOKEN_LEFT_BRACE))
			loopBody->stmts.emplace_back(ParseScopeStmt());
		else //for loop body is a expr stmt
			loopBody->stmts.emplace_back(ParseExprStmt());

		for (auto &independentVariable : independentVariables)
			loopBody->stmts.emplace_back(independentVariable);

		auto whileStmt = new WhileStmt(forCondition, loopBody);

		forStmt->stmts.emplace_back(whileStmt);

		return forStmt;
	}

	Stmt *Parser::ParseBreakStmt()
	{
		Consume(TOKEN_BREAK, "Expect 'break' keyword.");
		auto breakStmt = new BreakStmt();
		Consume(TOKEN_SEMICOLON, "Expect ';' after break stmt.");
		return breakStmt;
	}

	Stmt *Parser::ParseContinueStmt()
	{
		Consume(TOKEN_CONTINUE, "Expect 'continue' keyword.");
		auto continueStmt = new ContinueStmt();
		Consume(TOKEN_SEMICOLON, "Expect ';' after continue stmt.");
		return continueStmt;
	}

	Stmt *Parser::ParseFunctionStmt()
	{
		Consume(TOKEN_FUNCTION, "Expect 'function' keyword");

		auto funcStmt = new FunctionStmt();

		funcStmt->name = (IdentifierExpr *)ParseIdentifierExpr();

		Consume(TOKEN_LEFT_PAREN, "Expect '(' after function name.");

		if (!IsMatchCurToken(TOKEN_RIGHT_PAREN)) //has parameter
		{
			IdentifierExpr *idenExpr = (IdentifierExpr *)ParseIdentifierExpr();
			funcStmt->parameters.emplace_back(idenExpr);
			while (IsMatchCurTokenAndStepOnce(TOKEN_COMMA))
			{
				idenExpr = (IdentifierExpr *)ParseIdentifierExpr();
				funcStmt->parameters.emplace_back(idenExpr);
			}
		}
		Consume(TOKEN_RIGHT_PAREN, "Expect ')' after function expr's '('");

		funcStmt->body = (ScopeStmt *)ParseScopeStmt();

		return funcStmt;
	}

	Stmt *Parser::ParseClassStmt()
	{
		Consume(TOKEN_CLASS, "Expect 'class' keyword");
		auto classStmt = new ClassStmt();
		classStmt->name = (IdentifierExpr *)ParseIdentifierExpr();

		Consume(TOKEN_LEFT_BRACE, "Expect '{' after class name.");

		while (!IsMatchCurToken(TOKEN_RIGHT_BRACE))
		{
			if (IsMatchCurTokenAndStepOnce(TOKEN_PUBLIC)) //public function or variable
			{

				if (IsMatchCurToken(TOKEN_FUNCTION))
					classStmt->publicFunctions.emplace_back((FunctionStmt*)ParseFunctionStmt());
				else if (IsMatchCurToken(TOKEN_VAR))
					classStmt->publicVars.emplace_back((VarStmt*)ParseVarStmt());
				else
					Consume({TOKEN_FUNCTION, TOKEN_VAR}, "Expect 'function' or 'var' after 'public' keyword.");
			}
			else if (IsMatchCurTokenAndStepOnce(TOKEN_PROTECTED)) //protected function or variable
			{
				if (IsMatchCurToken(TOKEN_FUNCTION))
					classStmt->protectedFunctions.emplace_back((FunctionStmt*)ParseFunctionStmt());
				else if (IsMatchCurTokenAndStepOnce(TOKEN_VAR))
					classStmt->protectedVars.emplace_back((VarStmt*)ParseVarStmt());
				else
					Consume({TOKEN_FUNCTION, TOKEN_VAR}, "Expect 'function' or 'var' after 'protected' keyword.");
			}
			else //private function or variable,'private' keyword is not the necessary
			{
				if(IsMatchCurToken(TOKEN_PRIVATE))
					GetCurTokenAndStepOnce();
				if (IsMatchCurTokenAndStepOnce(TOKEN_FUNCTION))
					classStmt->privateFunctions.emplace_back((FunctionStmt*)ParseFunctionStmt());
				else if (IsMatchCurTokenAndStepOnce(TOKEN_VAR))
					classStmt->privateVars.emplace_back((VarStmt*)ParseVarStmt());
				else
					Consume({TOKEN_FUNCTION, TOKEN_VAR}, "only function or variable declaration is available in class scope.");
			}
		}

		Consume(TOKEN_RIGHT_BRACE, "Expect '}' after class name.");

		return classStmt;
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

		while (!IsMatchCurToken(TOKEN_SEMICOLON) && precedence < GetCurTokenPrecedence())
		{
			if (GetCurTokenPrecedence() != POSTFIX)
			{
				if (m_InfixFunctions.find(GetCurToken().type) == m_InfixFunctions.end())
					return leftExpr;

				auto infixFn = m_InfixFunctions[GetCurToken().type];

				leftExpr = (this->*infixFn)(leftExpr);
			}
			else
			{
				if (m_PostfixFunctions.find(GetCurToken().type) == m_PostfixFunctions.end())
					return leftExpr;

				auto postFixFn = m_PostfixFunctions[GetCurToken().type];

				leftExpr = (this->*postFixFn)(leftExpr);
			}
		}

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

	Expr *Parser::ParseGroupExpr()
	{
		Consume(TOKEN_LEFT_PAREN, "Expect '('.");
		auto groupExpr = new GroupExpr();
		groupExpr->expr = ParseExpr();
		Consume(TOKEN_RIGHT_PAREN, "Expect ')'.");
		return groupExpr;
	}

	Expr *Parser::ParseFunctionExpr()
	{
		Consume(TOKEN_FUNCTION, "Expect 'function' keyword");
		Consume(TOKEN_LEFT_PAREN, "Expect '(' after 'function' keyword");
		auto funcExpr = new FunctionExpr();

		if (!IsMatchCurToken(TOKEN_RIGHT_PAREN)) //has parameter
		{
			IdentifierExpr *idenExpr = (IdentifierExpr *)ParseIdentifierExpr();
			funcExpr->parameters.emplace_back(idenExpr);
			while (IsMatchCurTokenAndStepOnce(TOKEN_COMMA))
			{
				idenExpr = (IdentifierExpr *)ParseIdentifierExpr();
				funcExpr->parameters.emplace_back(idenExpr);
			}
		}
		Consume(TOKEN_RIGHT_PAREN, "Expect ')' after function expr's '('");

		funcExpr->body = (ScopeStmt *)ParseScopeStmt();

		return funcExpr;
	}

	Expr *Parser::ParseArrayExpr()
	{
		Consume(TOKEN_LEFT_BRACKET, "Expect '['.");

		auto arrayExpr = new ArrayExpr();
		if (!IsMatchCurToken(TOKEN_RIGHT_BRACKET))
		{
			//first element
			arrayExpr->elements.emplace_back(ParseExpr());
			while (IsMatchCurTokenAndStepOnce(TOKEN_COMMA))
				arrayExpr->elements.emplace_back(ParseExpr());
		}

		Consume(TOKEN_RIGHT_BRACKET, "Expect ']'.");

		return arrayExpr;
	}

	Expr *Parser::ParseTableExpr()
	{
		Consume(TOKEN_LEFT_BRACE, "Expect '{'.");

		auto tableExpr = new TableExpr();
		if (!IsMatchCurToken(TOKEN_RIGHT_BRACE))
		{
			//first element

			auto keyExpr = ParseExpr();
			Consume(TOKEN_COLON, "Expect ':' between table's key and value.");
			auto valueExpr = ParseExpr();
			tableExpr->elements[keyExpr] = valueExpr;
			while (IsMatchCurTokenAndStepOnce(TOKEN_COMMA))
			{
				auto keyExpr = ParseExpr();
				Consume(TOKEN_COLON, "Expect ':' between table's key and value.");
				auto valueExpr = ParseExpr();
				tableExpr->elements[keyExpr] = valueExpr;
			}
		}

		Consume(TOKEN_RIGHT_BRACE, "Expect '}'.");

		return tableExpr;
	}

	Expr *Parser::ParsePrefixExpr()
	{
		auto prefixExpr = new PrefixExpr();
		prefixExpr->op = GetCurTokenAndStepOnce().literal;
		prefixExpr->right = ParseExpr();
		return prefixExpr;
	}

	Expr *Parser::ParseInfixExpr(Expr *prefixExpr)
	{
		auto infixExpr = new InfixExpr();
		infixExpr->left = prefixExpr;
		infixExpr->op = GetCurTokenAndStepOnce().literal;
		infixExpr->right = ParseExpr(GetCurTokenPrecedence());
		return infixExpr;
	}

	Expr *Parser::ParsePostfixExpr(Expr *prefixExpr)
	{
		auto postfixExpr = new PostfixExpr();
		postfixExpr->left = prefixExpr;
		postfixExpr->op = GetCurTokenAndStepOnce().literal;
		return postfixExpr;
	}

	Expr *Parser::ParseTernaryExpr(Expr *prefixExpr)
	{
		auto ternaryExpr = new TernaryExpr();

		ternaryExpr->condition = prefixExpr;

		ternaryExpr->firstOp = Consume(TOKEN_QUESTION, "Expect '?'.").literal;

		ternaryExpr->trueBranch = ParseExpr();
		ternaryExpr->secondOp = Consume(TOKEN_COLON, "Expect ':'").literal;
		ternaryExpr->falseBranch = ParseExpr();
		return ternaryExpr;
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

	Precedence Parser::GetCurTokenPrecedence()
	{
		if (m_Precedence.find(GetCurToken().type) != m_Precedence.end())
			return m_Precedence[GetCurToken().type];
		return LOWEST;
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

	Precedence Parser::GetNextTokenPrecedence()
	{
		if (m_Precedence.find(GetNextToken().type) != m_Precedence.end())
			return m_Precedence[GetNextToken().type];
		return LOWEST;
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

	Token Parser::Consume(std::vector<TokenType> type, std::string errMsg)
	{
		for (const auto &t : type)
			if (IsMatchCurToken(t))
				return GetCurTokenAndStepOnce();
		std::cout << "[line " << GetCurToken().line << "]:" << errMsg << std::endl;
		return m_Tokens.back();
	}

	bool Parser::IsAtEnd()
	{
		return m_CurPos >= (int32_t)m_Tokens.size();
	}
}