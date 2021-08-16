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
		FloatNumExpr() :value(0.0) {}
		FloatNumExpr(double value) :value(value) {}
		~FloatNumExpr() {}

		std::string Stringify() override { return std::to_string(value); }
		AstType Type() override { return  AstType::FLOAT_NUM_EXPR; }

		double value;
	};

	struct IntNumExpr : public Expr
	{
		IntNumExpr() :value(0) {}
		IntNumExpr(int64_t value) :value(value) {}
		~IntNumExpr() {}

		std::string Stringify() override { return std::to_string(value); }
		AstType Type() override { return AstType::INT_NUM_EXPR; }

		int64_t value;
	};

	struct Stmt:public AstNode
	{
		Stmt() {}
		virtual ~Stmt() {}

		virtual std::string Stringify() = 0;
		virtual AstType Type() = 0;
	};
	struct AstStmts : public Stmt
	{
		AstStmts() {}
		AstStmts(std::vector<UniqueRef<Stmt>> stmts) :stmts(std::move(stmts)) {}
		~AstStmts() {}

		std::string Stringify() override
		{
			for (const auto& stmt : stmts)
				stmt->Stringify();
		}
		AstType Type() override { return  AstType::AST_STMTS; }

		std::vector<UniqueRef<Stmt>> stmts;
	};
}