#pragma once
#include <string>
#include <string>

#include <vector>
#include <memory>
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
		PREFIX_EXPR,
		INFIX_EXPR,
		POSTFIX_EXPR,

		VAR_STMT,
		EXPR_STMT,
		RETURN_STMT,
		IF_STMT,
		SCOPE_STMT,
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
		StrExpr(std::string str) : value(str) {}

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
		IdentifierExpr(std::string literal) : literal(literal) {}
		~IdentifierExpr() {}

		std::string Stringify() override { return literal; }
		AstType Type() override { return AstType::IDENTIFIER_EXPR; }

		std::string literal;
	};

	struct PrefixExpr : public Expr
	{
		PrefixExpr() : right(nullptr) {}
		PrefixExpr(std::string op, Expr *right) : op(op), right(right) {}
		~PrefixExpr()
		{
			delete right;
			right = nullptr;
		}

		std::string Stringify() override { return op + right->Stringify(); }
		AstType Type() override { return AstType::PREFIX_EXPR; }

		std::string op;
		Expr *right;
	};

	struct InfixExpr : public Expr
	{
		InfixExpr() : left(nullptr), right(nullptr) {}
		InfixExpr(std::string op, Expr *left, Expr *right) : op(op), left(left), right(right) {}
		~InfixExpr()
		{
			delete left;
			left = nullptr;

			delete right;
			right = nullptr;
		}

		std::string Stringify() override { return left->Stringify() + op + right->Stringify(); }
		AstType Type() override { return AstType::INFIX_EXPR; }

		std::string op;
		Expr *left;
		Expr *right;
	};

	struct PostfixExpr : public Expr
	{
		PostfixExpr() : left(nullptr) {}
		PostfixExpr(Expr* left,std::string op) : op(op), left(left) {}
		~PostfixExpr()
		{
			delete left;
			left = nullptr;
		}

		std::string Stringify() override { return left->Stringify()+op; }
		AstType Type() override { return AstType::POSTFIX_EXPR; }

		Expr *left;
		std::string op;
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
		ExprStmt() : expr(nullptr) {}
		ExprStmt(Expr *expr) : expr(expr) {}
		~ExprStmt()
		{
			delete expr;
			expr = nullptr;
		}

		std::string Stringify() override { return expr->Stringify() + ";"; }
		AstType Type() override { return AstType::EXPR_STMT; }

		Expr *expr;
	};

	struct VarStmt : public Stmt
	{
		VarStmt() {}
		VarStmt(std::unordered_map<IdentifierExpr *, Expr *> variables) : variables(variables) {}
		~VarStmt() {}

		std::string Stringify() override
		{
			std::string result = "var ";
			if (!variables.empty())
			{
				for (const auto &variable : variables)
					result += variable.first->Stringify() + "=" + variable.second->Stringify() + ",";
				result = result.substr(0, result.size() - 1);
			}
			result += ";";
			return result;
		}
		AstType Type() override { return AstType::VAR_STMT; }

		std::unordered_map<IdentifierExpr *, Expr *> variables;
	};

	struct ReturnStmt : public Stmt
	{
		ReturnStmt() : expr(nullptr) {}
		ReturnStmt(Expr *expr) : expr(expr) {}
		~ReturnStmt()
		{
			delete expr;
			expr = nullptr;
		}

		std::string Stringify() override { return "return " + expr->Stringify() + ";"; }
		AstType Type() override { return AstType::RETURN_STMT; }

		Expr *expr;
	};

	struct IfStmt : public Stmt
	{
		IfStmt() : condition(nullptr), thenBranch(nullptr), elseBranch(nullptr) {}
		IfStmt(Expr *condition, Stmt *thenBranch, Stmt *elseBranch)
			: condition(condition),
			  thenBranch(thenBranch),
			  elseBranch(elseBranch)
		{
		}
		~IfStmt()
		{
			delete condition;
			condition = nullptr;
			delete thenBranch;
			thenBranch = nullptr;
			delete elseBranch;
			elseBranch = nullptr;
		}

		std::string Stringify() override
		{
			std::string result;
			result = "if(" + condition->Stringify() + ")" + thenBranch->Stringify();
			if (elseBranch != nullptr)
				result += elseBranch->Stringify();
			return result;
		}
		AstType Type() override { return AstType::IF_STMT; }

		Expr *condition;
		Stmt *thenBranch;
		Stmt *elseBranch;
	};

	struct ScopeStmt : public Stmt
	{
		ScopeStmt() {}
		ScopeStmt(std::vector<Stmt *> stmts) : stmts(stmts) {}
		~ScopeStmt() { std::vector<Stmt *>().swap(stmts); }

		std::string Stringify() override
		{
			std::string result = "{";
			for (const auto &stmt : stmts)
				result += stmt->Stringify();
			result += "}";
			return result;
		}

		AstType Type() override { return AstType::SCOPE_STMT; }
		std::vector<Stmt *> stmts;
	};

	struct AstStmts : public Stmt
	{
		AstStmts() {}
		AstStmts(std::vector<Stmt *> stmts) : stmts(stmts) {}
		~AstStmts() { std::vector<Stmt *>().swap(stmts); }

		std::string Stringify() override
		{
			std::string result;
			for (const auto &stmt : stmts)
				result += stmt->Stringify();
			return result;
		}
		AstType Type() override { return AstType::AST_STMTS; }

		std::vector<Stmt *> stmts;
	};
}