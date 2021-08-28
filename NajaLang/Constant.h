#pragma once
#include "Ast.h"
#include "Object.h"
namespace NajaLang
{
    static NullExpr *nullExpr = new NullExpr();
    static TrueExpr *trueExpr = new TrueExpr();
    static FalseExpr *falseExpr = new FalseExpr();
    static ThisExpr *thisExpr = new ThisExpr();
    static BaseExpr *baseExpr = new BaseExpr();

    static TrueObject *trueObject = new TrueObject();
    static FalseObject *falseObject = new FalseObject();
    static NullObject *nullObject = new NullObject();
}