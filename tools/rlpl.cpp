#include <string>
#include <string>
#include <iostream>
#include "NajaLang.h"

void Rlpl()
{
    std::string line;
    NajaLang::Lexer lexer;
    std::cout << "> ";
    while (getline(std::cin, line))
    {
        auto tokens = lexer.ScanTokens(line);
        for (const auto &token : tokens)
            std::cout << token << std::endl;

        std::cout << "> ";
    }
}

void RunFile(std::string_view path)
{
    std::string content = NajaLang::ReadFile(path);
    NajaLang::Lexer lexer;
    auto tokens = lexer.ScanTokens(content);
    for (const auto &token : tokens)
        std::cout << token << std::endl;
}

int main(int argc, char **argv)
{
    if (argc == 2)
        RunFile(argv[1]);
    else if(argc==1)
        Rlpl();
    else 
        std::cout<<"Usage: rlpl [filepath]"<<std::endl;
    return 0;
}