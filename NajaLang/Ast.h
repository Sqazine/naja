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
		NULL_EXPR,
		TRUE_EXPR,
		FALSE_EXPR,
		IDENTIFIER_EXPR,

		VAR_STMT,
		EXPR_STMT,
		RETURN_STMT,
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

	struct NullExpr : public Expr
	{
		NullExpr() {}
		~NullExpr() {}

		std::string Stringify() override { return "null"; }
		AstType Type() override { return AstType::NULL_EXPR; }
	};

	struct TrueExpr : public Expr
	{
		TrueExpr() {}
		~TrueExpr() {}

		std::string Stringify() override { return "true"; }
		AstType Type() override { return AstType::TRUE_EXPR; }
	};

	struct FalseExpr : public Expr
	{
		FalseExpr() {}
		~FalseExpr() {}

		std::string Stringify() override { return "false"; }
		AstType Type() override { return AstType::FALSE_EXPR; }
	};

	struct IdentifierExpr : public Expr
	{
		IdentifierExpr() {}
		IdentifierExpr(std::string_view literal) : literal(literal) {}
		~IdentifierExpr() {}

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
		ExprStmt(SharedRef<Expr> expr) : expr(expr) {}

		std::string Stringify() override { return expr->Stringify() + ";"; }
		AstType Type() override { return AstType::EXPR_STMT; }

		SharedRef<Expr> expr;
	};

	struct VarStmt : public Stmt
	{
		VarStmt(){}
		VarStmt(std::unordered_map<SharedRef<IdentifierExpr>, SharedRef<Expr>> variables) : variables(variables) {}
		~VarStmt(){}

		std::string Stringify() override
		{
			std::string result = "var ";
			if (!variables.empty())
			{
				for (const auto &[key, value] : variables)
					result += key->Stringify() + "=" + value->Stringify() + ",";
				result = result.substr(0, result.size() - 1);
			}
			result += ";";
			return result;
		}
		AstType Type() override { return AstType::VAR_STMT; }

		std::unordered_map<SharedRef<IdentifierExpr>, SharedRef<Expr>> variables;
	};

	struct ReturnStmt:public Stmt
	{
		ReturnStmt() {}
		ReturnStmt(SharedRef<Expr> expr) : expr(expr) {}

		std::string Stringify() override { return "return "+expr->Stringify() + ";"; }
		AstType Type() override { return AstType::RETURN_STMT; }

		SharedRef<Expr> expr;
	};

	struct AstStmts : public Stmt
	{
		AstStmts() {}
		AstStmts(std::vector<SharedRef<Stmt>> stmts) : stmts(stmts) {}
		~AstStmts() {}

		std::string Stringify() override
		{
			std::string result;
			for (const auto &stmt : stmts)
				result += stmt->Stringify();
			return result;
		}
		AstType Type() override { return AstType::AST_STMTS; }

		std::vector<SharedRef<Stmt>> stmts;
	};
}