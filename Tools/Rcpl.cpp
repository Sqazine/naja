#include <string>
#include <string>
#include <iostream>
#include "NajaLang.h"
#include "Utils.h"
void Rcpl()
{
	std::string line;
	NajaLang::Lexer lexer;
	NajaLang::Parser parser;
	NajaLang::Compiler compiler;
	std::cout << "> ";
	while (getline(std::cin, line))
	{
		auto tokens = lexer.ScanTokens(line);
		auto stmt = parser.Parse(tokens);

		if (parser.HasError())
			parser.PrintErrors();

        auto chunk=compiler.Compile(stmt);

        std::cout<<chunk.Stringify()<<std::endl;

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

	if (parser.HasError())
			parser.PrintErrors();

	std::cout << stmts->Stringify() << std::endl;
}

int main(int argc, char **argv)
{
	if (argc == 2)
		RunFile(argv[1]);
	else if (argc == 1)
		Rcpl();
	else
		std::cout << "Usage: rcpl [filepath]" << std::endl;
	return 0;
}