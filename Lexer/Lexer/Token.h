#pragma once

class Token
{
public:
	std::string m_lexeme;
	TokenType m_type;
	size_t m_lineNumber;
	size_t m_pos;

	Token(std::string const& lexeme = "", 
		TokenType const& type = TokenType::ERROR, 
		size_t lineNumber = 0, size_t position = 0);

	std::string ToString() const
	{
		return std::string("Lexeme(\""+ m_lexeme + "\")   " + "Type(" + GetStringifiedType(m_type) + ")   " 
			+ "Line number(" + std::to_string(m_lineNumber) + ")   " + "Position("+ std::to_string(m_pos) + ")");
	}

	std::string GetStringifiedType(TokenType type) const
	{
		auto name = TOKEN_NAMES.at(type);
		return name;
	}

	void ClearToken()
	{
		m_lexeme = "";
		m_type = TokenType::ERROR;
		m_lineNumber = 0;
		m_pos = 0;
	}

	bool IsEmpty() const
	{
		return m_lexeme == "";
	}

	void AddSymbolToLexeme(char symbol)
	{
		m_lexeme.push_back(symbol);
	}


	void SetPosition(size_t lineNumber, size_t position)
	{
		m_lineNumber = lineNumber;
		m_pos = position;
	}

	size_t LexemeLength()
	{
		return m_lexeme.length();
	}

	std::string GetLexeme() const
	{
		return m_lexeme;
	}

	TokenType GetType()
	{
		return m_type;
	}

	size_t GetLineNumber()
	{
		return m_lineNumber;
	}

	size_t GetPosition()
	{
		return m_pos;
	}
};