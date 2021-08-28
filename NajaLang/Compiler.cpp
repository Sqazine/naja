#include "Compiler.h"
#include "Object.h"
#include "Constant.h"
namespace NajaLang
{
    Compiler::Compiler()
    {
    }
    Compiler::~Compiler()
    {
    }

    const Chunk &Compiler::Compile(Stmt *stmt)
    {
        CompileAstStmts((AstStmts *)stmt);
        return m_Chunk;
    }

    void Compiler::CompileAstStmts(AstStmts *stmt)
    {
        for (const auto &s : stmt->stmts)
            CompileStmt(s);
    }

    void Compiler::CompileStmt(Stmt *stmt)
    {
        switch (stmt->Type())
        {
        case RETURN_STMT:
            CompileReturnStmt((ReturnStmt *)stmt);
            break;
        default:
            break;
        }
    }
    void Compiler::CompileReturnStmt(ReturnStmt *stmt)
    {
        if (stmt->expr)
            CompileExpr(stmt->expr);
        m_Chunk.AddOpCode(RETURN_OP);
    }

    void Compiler::CompileExprStmt(ExprStmt *stmt)
    {
        CompileExpr(stmt->expr);
    }

    void Compiler::CompileExpr(Expr *expr)
    {
        switch (expr->Type())
        {
        case FLOAT_NUM_EXPR:
            CompileFloatNumExpr((FloatNumExpr *)expr);
            break;
        case INT_NUM_EXPR:
            CompileIntNumExpr((IntNumExpr *)expr);
            break;
        case TRUE_EXPR:
            CompileTrueExpr((TrueExpr*)expr);
            break;
        case FALSE_EXPR:
            CompileFalseExpr((FalseExpr*)expr);
            break;
        case NULL_EXPR:
            CompileNullExpr((NullExpr*)expr);
            break;
        default:
            break;
        }
    }

    void Compiler::CompileFloatNumExpr(FloatNumExpr *expr)
    {
        m_Chunk.AddOpCode(FLOAT_NUM_OP);
        size_t offset = m_Chunk.AddObject(new FloatNumObject(expr->value));
        m_Chunk.AddOpCode(offset);
    }

    void Compiler::CompileIntNumExpr(IntNumExpr *expr)
    {
        m_Chunk.AddOpCode(INT_NUM_OP);
        uint64_t offset = m_Chunk.AddObject(new IntNumObject(expr->value));
        m_Chunk.AddOpCode(offset);
    }

    void Compiler::CompileTrueExpr(TrueExpr *expr)
    {
         m_Chunk.AddOpCode(TRUE_OP);
        uint64_t offset = m_Chunk.AddObject(trueObject);
        m_Chunk.AddOpCode(offset);
    }
    void Compiler::CompileFalseExpr(FalseExpr *expr)
    {
         m_Chunk.AddOpCode(FALSE_OP);
        uint64_t offset = m_Chunk.AddObject(falseObject);
        m_Chunk.AddOpCode(offset);
    }
    void Compiler::CompileNullExpr(NullExpr *expr)
    {
          m_Chunk.AddOpCode(NULL_OP);
        uint64_t offset = m_Chunk.AddObject(nullObject);
        m_Chunk.AddOpCode(offset);
    }

}