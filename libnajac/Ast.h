#pragma once
#include <string>
namespace Najac
{
    enum AstType
    {

    };

    struct AstNode
    {
        AstNode() {}
        virtual ~AstNode() {}

        virtual std::string Stringify() = 0;
        virtual AstType AstType() = 0;
    };

    
}