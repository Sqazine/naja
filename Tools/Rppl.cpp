#include <string>
#include <string>
#include <iostream>
#include "NajaLang.h"
#include "Utils.h"
void Rppl()
{
	std::string line;
	NajaLang::Lexer lexer;
	NajaLang::Parser parser;
	std::cout << "> ";
	while (getline(std::cin, line))
	{
		auto tokens = lexer.ScanTokens(line);
		auto stmts = parser.Parse(tokens);

		std::cout << stmts->Stringify() << std::endl;

		std::cout << "> ";
	}
}

void RunFile(std::string path)
{
	std::string content = ReadFile(path);
	NajaLang::Lexer lexer;
	NajaLang::Parser parser;

	auto tokens = lexer.ScanTokens(content);

	auto stmts = parser.Parse(tokens);

	std::cout << stmts->Stringify() << std::endl;
}

int main(int argc, char** argv)
{
	// if (argc == 2)
	// 	RunFile(argv[1]);
	// else if (argc == 1)
	// 	Rppl();
	// else
	// 	std::cout << "Usage: rlpl [filepath]" << std::endl;

	RunFile("../class.naja");
	return 0;
}