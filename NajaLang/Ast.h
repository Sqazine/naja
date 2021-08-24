#pragma once
#include <string>
#include <map>

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
		THIS_EXPR,
		BASE_EXPR,
		GROUP_EXPR,
		FUNCTION_EXPR,
		ARRAY_EXPR,
		TABLE_EXPR,
		PREFIX_EXPR,
		INFIX_EXPR,
		POSTFIX_EXPR,
		TERNARY_EXPR,
		INDEX_EXPR,
		FUNCTION_CALL_EXPR,

		VAR_STMT,
		EXPR_STMT,
		RETURN_STMT,
		IF_STMT,
		SCOPE_STMT,
		WHILE_STMT,
		BREAK_STMT,
		CONTINUE_STMT,
		FUNCTION_STMT,
		CLASS_STMT,
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

	struct ThisExpr : public Expr
	{
		ThisExpr() {}
		~ThisExpr() {}

		std::string Stringify() override { return "this"; }
		AstType Type() override { return AstType::THIS_EXPR; }
	};

	struct BaseExpr : public Expr
	{
		BaseExpr() {}
		~BaseExpr() {}

		std::string Stringify() override { return "base"; }
		AstType Type() override { return AstType::BASE_EXPR; }
	};

	struct ArrayExpr : public Expr
	{
		ArrayExpr() {}
		ArrayExpr(std::vector<Expr *> elements) : elements(elements) {}
		~ArrayExpr()
		{
			std::vector<Expr *>().swap(elements);
		}

		std::string Stringify() override
		{
			std::string result = "[";

			if (!elements.empty())
			{
				for (auto e : elements)
					result += e->Stringify() + ",";
				result = result.substr(0, result.size() - 1);
			}
			result += "]";
			return result;
		}
		AstType Type() override { return AstType::ARRAY_EXPR; }

		std::vector<Expr *> elements;
	};

	struct TableExpr : public Expr
	{
		TableExpr() {}
		TableExpr(std::map<Expr *, Expr *> elements) : elements(elements) {}
		~TableExpr() { std::map<Expr *, Expr *>().swap(elements); }

		std::string Stringify() override
		{
			std::string result = "{";

			if (!elements.empty())
			{
				for (auto e : elements)
					result += e.first->Stringify() + ":" + e.second->Stringify() + ",";
				result = result.substr(0, result.size() - 1);
			}
			result += "}";
			return result;
		}
		AstType Type() override { return AstType::TABLE_EXPR; }

		std::map<Expr *, Expr *> elements;
	};

	struct GroupExpr : public Expr
	{
		GroupExpr() {}
		GroupExpr(Expr *expr) : expr(expr) {}
		~GroupExpr() {}

		std::string Stringify() override { return "(" + expr->Stringify() + ")"; }
		AstType Type() override { return AstType::GROUP_EXPR; }

		Expr *expr;
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
		PostfixExpr(Expr *left, std::string op) : op(op), left(left) {}
		~PostfixExpr()
		{
			delete left;
			left = nullptr;
		}

		std::string Stringify() override { return left->Stringify() + op; }
		AstType Type() override { return AstType::POSTFIX_EXPR; }

		Expr *left;
		std::string op;
	};

	struct TernaryExpr : public Expr
	{
		TernaryExpr() : condition(nullptr), trueBranch(nullptr), falseBranch(nullptr) {}
		TernaryExpr(std::string firstOp, std::string secondOp, Expr *condition, Expr *trueBranch, Expr *falseBranch) : firstOp(firstOp), secondOp(secondOp), condition(condition), trueBranch(trueBranch), falseBranch(falseBranch)
		{
		}
		~TernaryExpr()
		{
			delete condition;
			condition = nullptr;
			delete trueBranch;
			trueBranch = nullptr;
			delete falseBranch;
			falseBranch = nullptr;
		};

		std::string Stringify() override { return condition->Stringify() + firstOp + trueBranch->Stringify() + secondOp + falseBranch->Stringify(); }
		AstType Type() override { return AstType::TERNARY_EXPR; }

		std::string firstOp;
		std::string secondOp;
		Expr *condition;
		Expr *trueBranch;
		Expr *falseBranch;
	};

	struct IndexExpr : public Expr
	{
		IndexExpr() {}
		IndexExpr(Expr *array, Expr *index) : array(array), index(index) {}
		~IndexExpr()
		{
			delete array;
			array = nullptr;
			delete index;
			index = nullptr;
		}
		std::string Stringify() override { return array->Stringify() + "[" + index->Stringify() + "]"; }

		AstType Type() override { return AstType::INDEX_EXPR; }

		Expr *array;
		Expr *index;
	};

	struct FunctionCallExpr : public Expr
	{
		FunctionCallExpr() {}
		FunctionCallExpr(Expr *function, std::vector<Expr *> arguments) : function(function), arguments(arguments) {}
		~FunctionCallExpr() {}

		std::string Stringify() override
		{
			std::string result = function->Stringify() + "(";

			if (!arguments.empty())
			{
				for (const auto &arg : arguments)
					result += arg->Stringify() + ",";
				result = result.substr(0, result.size() - 1);
			}
			result+=")";
			return result;
		}
		AstType Type() override { return AstType::FUNCTION_CALL_EXPR; }

		Expr *function;
		std::vector<Expr *> arguments;
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
		~VarStmt() { std::unordered_map<IdentifierExpr *, Expr *>().swap(variables); }

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

	struct FunctionExpr : public Expr
	{
		FunctionExpr() : body(nullptr) {}
		FunctionExpr(std::vector<IdentifierExpr *> parameters, ScopeStmt *body) : parameters(parameters), body(body) {}
		~FunctionExpr()
		{
			std::vector<IdentifierExpr *>().swap(parameters);
			delete body;
			body = nullptr;
		}

		std::string Stringify() override
		{
			std::string result = "function(";
			if (!parameters.empty())
			{
				for (auto param : parameters)
					result += param->Stringify() + ",";
				result = result.substr(0, result.size() - 1);
			}
			result += ")";
			result += body->Stringify();
			return result;
		}
		AstType Type() override { return AstType::FUNCTION_EXPR; }

		std::vector<IdentifierExpr *> parameters;
		ScopeStmt *body;
	};

	struct WhileStmt : public Stmt
	{
		WhileStmt() : condition(nullptr), stmt(nullptr) {}
		WhileStmt(Expr *condition, Stmt *stmt)
			: condition(condition),
			  stmt(stmt)
		{
		}
		~WhileStmt()
		{
			delete condition;
			condition = nullptr;
			delete stmt;
			stmt = nullptr;
		}

		std::string Stringify() override
		{
			return "while(" + condition->Stringify() + ")" + stmt->Stringify();
		}
		AstType Type() override { return AstType::WHILE_STMT; }

		Expr *condition;
		Stmt *stmt;
	};

	struct BreakStmt : public Stmt
	{
		BreakStmt() {}
		~BreakStmt() {}

		std::string Stringify() override { return "break;"; }
		AstType Type() override { return AstType::BREAK_STMT; }
	};

	struct ContinueStmt : public Stmt
	{
		ContinueStmt() {}
		~ContinueStmt() {}

		std::string Stringify() override { return "continue;"; }
		AstType Type() override { return AstType::CONTINUE_STMT; }
	};

	struct FunctionStmt : public Stmt
	{
		FunctionStmt() : name(nullptr), body(nullptr) {}
		FunctionStmt(IdentifierExpr *name, std::vector<IdentifierExpr *> parameters, ScopeStmt *body) : name(name), parameters(parameters), body(body) {}
		~FunctionStmt()
		{
			delete name;
			name = nullptr;

			std::vector<IdentifierExpr *>().swap(parameters);

			delete body;
			body = nullptr;
		}

		std::string Stringify() override
		{
			std::string result = "function " + name->Stringify() + "(";
			if (!parameters.empty())
			{
				for (auto param : parameters)
					result += param->Stringify() + ",";
				result = result.substr(0, result.size() - 1);
			}
			result += ")";
			result += body->Stringify();
			return result;
		}
		AstType Type() override { return AstType::FUNCTION_STMT; }

		IdentifierExpr *name;
		std::vector<IdentifierExpr *> parameters;
		ScopeStmt *body;
	};

	struct ClassStmt : public Stmt
	{
		ClassStmt() {}
		ClassStmt(IdentifierExpr *name,
				  std::vector<IdentifierExpr *> publicInherits, std::vector<IdentifierExpr *> protectedInherits, std::vector<IdentifierExpr *> privateInherits,
				  std::vector<VarStmt *> publicVars, std::vector<VarStmt *> protectedVars, std::vector<VarStmt *> privateVars,
				  std::vector<FunctionStmt *> publicFunctions, std::vector<FunctionStmt *> protectedFunctions, std::vector<FunctionStmt *> privateFunctions)
			: publicInherits(publicInherits), protectedInherits(protectedInherits), privateInherits(privateInherits),
			  publicVars(publicVars), protectedVars(protectedVars), privateVars(privateVars),
			  publicFunctions(publicFunctions), protectedFunctions(protectedFunctions), privateFunctions(privateFunctions)
		{
		}

		~ClassStmt()
		{
			delete name;
			name = nullptr;

			std::vector<IdentifierExpr *>().swap(publicInherits);
			std::vector<IdentifierExpr *>().swap(protectedInherits);
			std::vector<IdentifierExpr *>().swap(privateInherits);

			std::vector<VarStmt *>().swap(publicVars);
			std::vector<VarStmt *>().swap(protectedVars);
			std::vector<VarStmt *>().swap(privateVars);

			std::vector<FunctionStmt *>().swap(publicFunctions);
			std::vector<FunctionStmt *>().swap(protectedFunctions);
			std::vector<FunctionStmt *>().swap(privateFunctions);
		}

		std::string Stringify() override
		{
			std::string result = "class " + name->Stringify();

			if (!publicInherits.empty() || !protectedInherits.empty() || !privateInherits.empty())
			{
				result += ":";
				if (!publicInherits.empty())
					for (auto pubIn : publicInherits)
						result += "public " + pubIn->Stringify() + ",\n";
				if (!protectedInherits.empty())
					for (auto proIn : protectedInherits)
						result += "protected " + proIn->Stringify() + ",\n";

				if (!privateInherits.empty())
					for (auto priIn : privateInherits)
						result += "private " + priIn->Stringify() + ",\n";

				result = result.substr(0, result.size() - 2);
			}
			result += "\n{\n";

			if (!publicFunctions.empty())
				for (auto pubFn : publicFunctions)
					result += "\tpublic " + pubFn->Stringify() + "\n";

			if (!protectedFunctions.empty())
				for (auto proFn : protectedFunctions)
					result += "\tprotected " + proFn->Stringify() + "\n";

			if (!privateFunctions.empty())
				for (auto priFn : privateFunctions)
					result += "\tprivate " + priFn->Stringify() + "\n";

			if (!publicVars.empty())
				for (auto pubVar : publicVars)
					result += "\tpublic " + pubVar->Stringify() + "\n";

			if (!protectedVars.empty())
				for (auto proVar : protectedVars)
					result += "\tprotected " + proVar->Stringify() + "\n";

			if (!privateVars.empty())
				for (auto priVar : privateVars)
					result += "private " + priVar->Stringify() + "\n";

			result += "}\n";
			return result;
		}
		AstType Type() override { return AstType::CLASS_STMT; }

		IdentifierExpr *name;

		std::vector<VarStmt *> publicVars;
		std::vector<VarStmt *> protectedVars;
		std::vector<VarStmt *> privateVars;

		std::vector<FunctionStmt *> publicFunctions;
		std::vector<FunctionStmt *> protectedFunctions;
		std::vector<FunctionStmt *> privateFunctions;

		std::vector<IdentifierExpr *> publicInherits;
		std::vector<IdentifierExpr *> protectedInherits;
		std::vector<IdentifierExpr *> privateInherits;
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