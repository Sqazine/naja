#pragma once
#include "Ast.h"
class Semanticer
{
public:
    Semanticer();
    ~Semanticer();

    bool Check(Stmt *stmtTree);

private:
};