#pragma once
#include "Ast.h"
#include "Chunk.h"
namespace NajaLang
{

    class Compiler
    {
    public:
        Compiler();
        ~Compiler();

        const Chunk& Compile(Stmt* stmt);
    private:
        void CompileAstStmts(AstStmts* stmt);
        void CompileStmt(Stmt* stmt);
        void CompileReturnStmt(ReturnStmt* stmt);
        void CompileExprStmt(ExprStmt* stmt);

        void CompileExpr(Expr* expr);
        void CompileFloatNumExpr(FloatNumExpr* expr);
        void CompileIntNumExpr(IntNumExpr* expr);
        void CompileTrueExpr(TrueExpr* expr);
        void CompileFalseExpr(FalseExpr* expr);
        void CompileNullExpr(NullExpr* expr);

        Chunk m_Chunk;
    };
}