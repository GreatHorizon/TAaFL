#pragma once
#include <istream>
#include "Utils.h"
#include "Token.h"
#include "State.h"

class Lexer
{
public:
	Lexer(std::istream& input);
	void Run();
	const std::vector<Token>& GetTokensList();
private:
	void ProcessLine();
	void ProcessLexeme(char symbol);
	void FlushToken();
	void AddToken(Token token);
	char GetNextStreamSymbol();
	void ProcessComparisionOperator(char symbol);
	void ProcessLogicalOperator(char symbol);
	void SkipComment(char symbol);
	void SkipIgnored(char symbol);
	void GetStringLexeme();

	TokenType GetTokenTypeByString(std::string const& lexeme);
	TokenType GetTokenBySymbol(std::string const& lexeme);

	bool IsIdentifier(std::string const& lexeme);
	bool IsSeparator(std::string lexeme);
	bool IsKeyword(std::string const& lexeme);
	bool IsIgnoredSeparator(char symbol);
	bool IsLogicalOperator(char symbol);
	bool IsRelationOperator(std::string const& lexeme);
	bool IsArithmeticOperator(std::string const& lexeme);
	bool IsFloat(std::string const& lexeme) const;
	bool IsComment(char symbol);
	bool IsNumber(std::string const& lexeme, int base)const;

	bool IsBinaryNumber(std::string const& lexeme) const;
	bool IsDecimalNumber(std::string const& lexeme) const;
	bool IsHexadecimalNumber(std::string const& lexeme) const;
	bool IsOctalNumber(std::string const& lexeme) const;
	bool IsBoolean(std::string const& lexeme) const;
	bool IsExpFloat() const;
private:
	std::istream& m_iStream;
	std::vector<Token> m_tokens;
	std::string m_currentString;
	Token m_currentToken;
	size_t m_lineNumber = 0;
	size_t m_position = 0;

	State m_state = State::DEFAULT_STATE;


};
