#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>

const std::set<std::string> ARITHMETIC_OPERATORS = {
	"*"
	, "/"
	, "+"
	, "-"
};

const std::set<std::string> BOOLEANS = {
	"true"
	, "false"
};

const std::set<std::string> KEYWORDS = {
	"while"
	, "for"
	, "class"
	, "struct"
	, "public"
	, "private"
	, "static"
	, "string"
	, "int"
	, "float"
	, "enum"
	, "bool"
	, "break"
	, "continue"
	, "return"
	, "if"
};

const std::set<std::string> RELATION_OPERATORS = {
	"=="
	, ">"
	, "<"
	, "<="
	, ">="
	, "!="
};

const std::set<std::string> LOGICAL_OPEARTORS = {
	"&&"
	, "||"
};


const std::set<std::string> SEPARATORS = {
	","
	, ";"
	, ":"
};

const std::set<std::string> IGNORED_SEPARATORS = {
	" "
	, "\n"
	, "\t"
};

enum class TokenType
{
	ERROR,
	DECIMAL_NUMBER,
	HEX_NUMBER,
	OCTAL_NUMBER,
	BINARY_NUMBER,
	IDENTIFIER,
	STRING,
	FLOAT,
	BOOLEAN,
	ARITHMETIC_OPERATOR,
	IF_CONDITION,
	ELSE_CONDITION,
	SEPARATOR,
	BLOCK_OPEN_BRACKET,
	BLOCK_CLOSE_BRACKET,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	LOGICAL_OPERATOR,
	COMPARISION_OPERATOR,
	KEYWORD,
	ASSIGNMENT
};

const std::map<TokenType, std::string> TOKEN_NAMES = {
	{TokenType::ERROR, "Error"}
	, {TokenType::BOOLEAN, "Boolean"}
	, {TokenType::SEPARATOR, "Separator"}
	, {TokenType::STRING, "String"}
	, {TokenType::FLOAT, "Float"}
	, {TokenType::ARITHMETIC_OPERATOR, "Operator"}
	, {TokenType::IDENTIFIER, "Identifier"}
	, {TokenType::DECIMAL_NUMBER, "Decimal number"}
	, {TokenType::HEX_NUMBER, "Hexamecimal number"}
	, {TokenType::OCTAL_NUMBER, "Octal number"}
	, {TokenType::BINARY_NUMBER, "Binary number"}
	, {TokenType::BLOCK_OPEN_BRACKET, "Block open bracket"}
	, {TokenType::BLOCK_CLOSE_BRACKET, "Block close bracket"}
	, {TokenType::OPEN_BRACKET, "Open bracket"}
	, {TokenType::CLOSE_BRACKET, "Close bracket"}
	, {TokenType::LOGICAL_OPERATOR, "Logical operator"}
	, {TokenType::COMPARISION_OPERATOR, "Comparision operator"}
	, {TokenType::KEYWORD, "Keyword"}
	, {TokenType::ASSIGNMENT, "Assignment"}
};

const char BLOCK_OPEN_BRACKET = '{';
const char BLOCK_CLOSE_BRACKET = '}';
const char OPEN_BRACKET = '(';
const char CLOSE_BRACKET = ')';