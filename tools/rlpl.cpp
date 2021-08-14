#include <string>
#include <string>
#include <iostream>
#include "libnajac.h"

void Rlpl()
{
    std::string line;
    NajaC::Lexer lexer;
    std::cout << "> " << std::endl;
    while (getline(std::cin, line))
    {
        auto tokens = lexer.ScanTokens(line);
        for (const auto &token : tokens)
            std::cout << token << std::endl;

        std::cout << "> " << std::endl;
    }
}

int main(int argc, char **argv)
{
    Rlpl();
    return 0;
}