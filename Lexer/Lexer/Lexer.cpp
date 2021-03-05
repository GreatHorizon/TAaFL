#include "Lexer.h"
#include <sstream>
#include <regex>
#include <string>

void Lexer::Run()
{
	std::string line;
	while (!m_iStream.eof())
	{
		std::getline(m_iStream, m_currentString);

		if (m_currentString != "")
		{
			ProcessLine();
		}

		m_lineNumber++;
		FlushToken();
	}
}

void Lexer::ProcessLine()
{
	for (; m_position < m_currentString.length(); m_position++)
	{
		if (m_state == State::MULTILINE_COMMENT)
		{
			if (m_currentString[m_position] == '*' &&  GetNextStreamSymbol() == '/')
			{
				m_state = State::DEFAULT_STATE;
				m_position++;
			}
		}
		else
		{
			ProcessLexeme(m_currentString[m_position]);
		}
	}

	m_position = 0;
}


void Lexer::ProcessComparisionOperator(char symbol)
{
	if (GetNextStreamSymbol() == '=')
	{
		AddToken(Token(std::string(1, symbol) + "=", TokenType::COMPARISION_OPERATOR, m_lineNumber, m_position));
		m_position++;
	}
	else if (symbol == '=')
	{
		AddToken(Token("=", TokenType::ASSIGNMENT, m_lineNumber, m_position));
	}
	else
	{
		AddToken(Token(std::string(1, symbol), TokenType::COMPARISION_OPERATOR, m_lineNumber, m_position));
	}
}

void Lexer::ProcessLogicalOperator(char symbol)
{
	std::string lexeme;
	if (symbol == '&' && GetNextStreamSymbol() == '&')
	{
		lexeme = "&&";
	}
	else if (symbol == '|' && GetNextStreamSymbol() == '|')
	{
		lexeme = "||";
	}
	else
	{
		m_currentToken.AddSymbolToLexeme(symbol);
		return;
	}

	FlushToken();
	AddToken(Token(lexeme, TokenType::LOGICAL_OPERATOR, m_lineNumber, m_position));
	m_position++;
}

void Lexer::SkipComment(char symbol)
{
	if (symbol == '/')
	{
		if (GetNextStreamSymbol() == '/')
		{
			m_position = m_currentString.length() - 1;
		}
		else if (GetNextStreamSymbol() == '*')
		{
			m_state = State::MULTILINE_COMMENT;
			m_position++;
		}
	}
}

bool Lexer::IsComment(char symbol)
{
	return symbol == '/' && (GetNextStreamSymbol() == '/' || GetNextStreamSymbol() == '*');
}

void Lexer::GetStringLexeme()
{
	std::string str;
	for (size_t i = m_position + 1; i < m_currentString.length(); i++)
	{
		if (m_currentString[i] == '"')
		{
			AddToken(Token(str, TokenType::STRING, m_lineNumber, m_position));
			m_position = i;
			return;
		}

		str.push_back(m_currentString[i]);
	}

	AddToken(Token("\"", TokenType::ERROR, m_lineNumber, m_position));
}

void Lexer::SkipIgnored(char symbol)
{
	if (symbol == '\\' && GetNextStreamSymbol() == 'n' || GetNextStreamSymbol() == 't')
	{
		m_position++;
	}
}

void Lexer::ProcessLexeme(char symbol)
{
	if (symbol == '"')
	{
		FlushToken();
		GetStringLexeme();
	}
	else if (IsComment(symbol))
	{
		FlushToken();
		SkipComment(symbol);
	}
	else if (IsSeparator(std::string(1, symbol)) || IsArithmeticOperator(std::string(1, symbol)) || symbol == BLOCK_OPEN_BRACKET || 
		symbol == BLOCK_CLOSE_BRACKET || symbol == OPEN_BRACKET || symbol == CLOSE_BRACKET)
	{
		if ((symbol == '+' || symbol == '-') && IsExpFloat())
		{
			m_currentToken.AddSymbolToLexeme(symbol);
			return;
		}

		FlushToken();
		AddToken(Token(std::string(1, symbol), GetTokenBySymbol(std::string(1, symbol)), m_lineNumber, m_position));

	}
	else if (symbol == ' ' || symbol == '\t')
	{
		FlushToken();
	}
	else if (symbol == '!' || symbol == '=' || symbol == '>' || symbol == '<')
	{
		FlushToken();
		ProcessComparisionOperator(symbol);
	}
	else if (symbol == '|' || symbol == '&')
	{
		ProcessLogicalOperator(symbol);
	}
	else
	{
		if (m_currentToken.IsEmpty())
		{
			m_currentToken.SetPosition(m_lineNumber, m_position);
		}

		m_currentToken.AddSymbolToLexeme(symbol);
	}
}

char Lexer::GetNextStreamSymbol()
{
	if (m_currentString.length() == m_position + 1)
	{
		return '\0';
	}

	return m_currentString[m_position + 1];
}

bool Lexer::IsIgnoredSeparator(char symbol)
{
	if (symbol == ' ' || symbol == '\\' && (GetNextStreamSymbol() == 'n' || GetNextStreamSymbol() == 't'))
	{
		return true;
	}

	return false;
}

bool Lexer::IsLogicalOperator(char symbol)
{
	return LOGICAL_OPEARTORS.end() != LOGICAL_OPEARTORS.find(std::string(1,	symbol));
}

void Lexer::FlushToken()
{
	if (!m_currentToken.IsEmpty())
	{
		m_currentToken.m_type = GetTokenTypeByString(m_currentToken.m_lexeme);
		m_tokens.push_back(m_currentToken);
		m_currentToken.ClearToken();
	}
}

void Lexer::AddToken(Token token)
{
	m_tokens.push_back(token);
}

TokenType Lexer::GetTokenTypeByString(std::string const& lexeme)
{
	if (IsKeyword(lexeme)) return TokenType::KEYWORD;
	if (IsBoolean(lexeme)) return TokenType::BOOLEAN;
	else if (IsIdentifier(lexeme)) return TokenType::IDENTIFIER;
	else if (IsRelationOperator(lexeme)) return TokenType::COMPARISION_OPERATOR;
	else if (IsBinaryNumber(lexeme)) return TokenType::BINARY_NUMBER;
	else if (IsHexadecimalNumber(lexeme)) return TokenType::HEX_NUMBER;
	else if (IsOctalNumber(lexeme)) return TokenType::OCTAL_NUMBER;
	else if (IsDecimalNumber(lexeme)) return TokenType::DECIMAL_NUMBER;
	else if (IsFloat(lexeme)) return TokenType::FLOAT;
	else return TokenType::ERROR;
}

TokenType Lexer::GetTokenBySymbol(std::string const& lexeme)
{
	if (lexeme == "{") return TokenType::BLOCK_OPEN_BRACKET;
	else if (lexeme == "}") return TokenType::BLOCK_CLOSE_BRACKET;
	else if (lexeme == "(") return TokenType::OPEN_BRACKET;
	else if (lexeme == ")") return TokenType::CLOSE_BRACKET;
	else if (lexeme == "=") return TokenType::ASSIGNMENT;
	else if (lexeme == ")") return TokenType::CLOSE_BRACKET;
	else if (IsSeparator(lexeme)) return TokenType::SEPARATOR;
	else if (IsArithmeticOperator(lexeme)) return TokenType::ARITHMETIC_OPERATOR;
	else return TokenType::ERROR;
}

bool Lexer::IsSeparator(std::string lexeme)
{
	return SEPARATORS.end() != SEPARATORS.find(lexeme);
}

bool Lexer::IsBoolean(std::string const& lexeme) const
{
	return BOOLEANS.end() != BOOLEANS.find(lexeme);
}

bool Lexer::IsKeyword(std::string const& lexeme)
{
	return KEYWORDS.end() != KEYWORDS.find(lexeme);
}

bool Lexer::IsRelationOperator(std::string const& lexeme)
{
	return RELATION_OPERATORS.end() != RELATION_OPERATORS.find(lexeme);
}

bool Lexer::IsArithmeticOperator(std::string const& lexeme)
{
	return ARITHMETIC_OPERATORS.end() != ARITHMETIC_OPERATORS.find(lexeme);
}

bool Lexer::IsFloat(std::string const& lexeme) const
{
	if (lexeme.find('.') == std::string::npos && lexeme.find('e') == std::string::npos)
	{
		return false;
	}

	size_t foundIndex;

	try
	{
		auto number = std::stof(lexeme, &foundIndex);
	}
	catch (const std::exception&)
	{
		return false;
	}


	if (lexeme[foundIndex] != '\0')
	{
		return false;
	}

	return true;
}


bool Lexer::IsNumber(std::string const& lexeme, int base) const
{
	size_t foundIndex;

	try
	{
		auto number = std::stoull(lexeme, &foundIndex, base);
	}
	catch (const std::exception&)
	{
		return false;
	}


	if (lexeme[foundIndex] != '\0')
	{
		return false;
	}

	return true;
}

bool Lexer::IsIdentifier(std::string const& lexeme)
{
	if (!(lexeme[0] >= 'a' && lexeme[0] <= 'z'
		|| lexeme[0] >= 'A' && lexeme[0] <= 'Z'
		|| lexeme[0] == '_'))
	{
		return false;
	}

	for (size_t i = 1; i < lexeme.length(); i++)
	{
		if (!(lexeme[i] >= 'a' && lexeme[i] <= 'z'
			|| lexeme[i] >= 'A' && lexeme[i] <= 'Z'
			|| lexeme[i] >= '0' && lexeme[i] <= '9'
			|| lexeme[i] == '_'))
		{
			return false;
		}
	}

	return true;
}

bool Lexer::IsBinaryNumber(std::string const& lexeme) const
{
	if (!(lexeme.length() > 2 && lexeme[0] == '0' && lexeme[1] == 'b'))
	{
		return false;
	}

	return IsNumber(lexeme.substr(2, std::string::npos), 2);
}

bool Lexer::IsDecimalNumber(std::string const& lexeme) const
{
	return IsNumber(lexeme, 10);
}

bool Lexer::IsHexadecimalNumber(std::string const& lexeme) const
{
	if (!(lexeme.length() > 2 && lexeme[0] == '0' && lexeme[1] == 'x'))
	{
		return false;
	}

	return IsNumber(lexeme, 16);
}

bool Lexer::IsOctalNumber(std::string const& lexeme) const
{
	if (!(lexeme.length() > 1 && lexeme[0] == '0'))
	{
		return false;
	}

	return IsNumber(lexeme, 8);
}



bool Lexer::IsExpFloat() const
{
	if (m_currentToken.m_lexeme.size() > 2 && m_currentToken.m_lexeme[m_currentToken.m_lexeme.size() - 1] == 'e')
	{
		return IsFloat(m_currentToken.m_lexeme.substr(0, m_currentToken.m_lexeme.size() - 1));
	}

	return false;
}


Lexer::Lexer(std::istream& input)
	: m_iStream(input)
{
}
const std::vector<Token>& Lexer::GetTokensList()
{
	return m_tokens;
}

Token::Token(std::string const& lexeme, TokenType const& type, size_t lineNumber, size_t position)
	: m_lexeme(lexeme)
	, m_type(type)
	, m_lineNumber(lineNumber)
	, m_pos(position)
{
};
