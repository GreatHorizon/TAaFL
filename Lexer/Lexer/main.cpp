#include <iostream>
#include <fstream>
#include "Lexer.h"
#include <sstream>


void PrintTokens(const std::vector<Token>& tokens, std::ostream& out)
{
	//std::cout << "Lexeme   ---    Type   ---   Line number   ---   Position\n";

	if (tokens.size() == 0)
	{
		out << "EOF";
		return;
	}

	int i = 1;
	for (auto& token : tokens)
	{
		out << i << " - " << token.ToString() << "\n\n";
		++i;
	}
}



int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid arguments count\n";
		return 1;
	}

	std::ifstream input(argv[1]);
	std::ofstream output(argv[2]);

	Lexer lexer(input);
	lexer.Run();
	PrintTokens(lexer.GetTokensList(), output);
	
	return 0;
}