#pragma once
#include <string>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include "Utils.h"
namespace NajaLang
{
	enum class AstType
	{
		FLOAT_NUM_EXPR,
		INT_NUM_EXPR,
		STR_EXPR,
		IDENTIFIER_EXPR,

		EXPR_STMT,
		AST_STMTS,
	};

	struct AstNode
	{
		AstNode() {}
		virtual ~AstNode() {}

		virtual std::string Stringify() = 0;
		virtual AstType Type() = 0;
	};

	struct Expr
	{
		Expr() {}
		virtual ~Expr() {}

		virtual std::string Stringify() = 0;
		virtual AstType Type() = 0;
	};

	struct FloatNumExpr : public Expr
	{
		FloatNumExpr() : value(0.0) {}
		FloatNumExpr(double value) : value(value) {}
		~FloatNumExpr() {}

		std::string Stringify() override { return std::to_string(value); }
		AstType Type() override { return AstType::FLOAT_NUM_EXPR; }

		double value;
	};

	struct IntNumExpr : public Expr
	{
		IntNumExpr() : value(0) {}
		IntNumExpr(int64_t value) : value(value) {}
		~IntNumExpr() {}

		std::string Stringify() override { return std::to_string(value); }
		AstType Type() override { return AstType::INT_NUM_EXPR; }

		int64_t value;
	};

	struct StrExpr : public Expr
	{
		StrExpr() {}
		StrExpr(std::string_view str) : value(str) {}

		std::string Stringify() override { return value; }
		AstType Type() override { return AstType::STR_EXPR; }

		std::string value;
	};

	struct IdentifierExpr:public Expr
	{
		IdentifierExpr(){}
		IdentifierExpr(std::string_view literal):literal(literal){}
		~IdentifierExpr(){}

		std::string Stringify() override { return literal; }
		AstType Type() override { return AstType::IDENTIFIER_EXPR; }
		
		std::string literal;
	};

	struct Stmt : public AstNode
	{
		Stmt() {}
		virtual ~Stmt() {}

		virtual std::string Stringify() = 0;
		virtual AstType Type() = 0;
	};

	struct ExprStmt : public Stmt
	{
		ExprStmt() {}
		ExprStmt(UniqueRef<Expr> expr) : expr(std::move(expr)) {}

		std::string Stringify() override { return expr->Stringify() + ";"; }
		AstType Type() override { return AstType::EXPR_STMT; }

		UniqueRef<Expr> expr;
	};

	struct AstStmts : public Stmt
	{
		AstStmts() {}
		AstStmts(std::vector<UniqueRef<Stmt>> stmts) : stmts(std::move(stmts)) {}
		~AstStmts() {}

		std::string Stringify() override
		{
			std::string result;
			for (const auto &stmt : stmts)
				result += stmt->Stringify();
			return result;
		}
		AstType Type() override { return AstType::AST_STMTS; }

		std::vector<UniqueRef<Stmt>> stmts;
	};
}