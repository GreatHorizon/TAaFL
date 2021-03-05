
#include "stdafx.h"
#include "../Lexer/Utils.h"
#include "../Lexer/Token.h"
#include "../Lexer/Lexer.h"
#include <utility>

void CheckLexemeList(std::vector<Token> actualList, std::vector<Token> expectedList)
{
	BOOST_CHECK_EQUAL(actualList.size(), expectedList.size());

	for (size_t i = 0; i < actualList.size(); i++)
	{
		BOOST_CHECK_EQUAL(actualList[i].GetLexeme() , expectedList[i].GetLexeme());
		BOOST_CHECK(actualList[i].GetType() ==  expectedList[i].GetType());
		BOOST_CHECK_EQUAL(actualList[i].GetLineNumber(), expectedList[i].GetLineNumber());
		BOOST_CHECK_EQUAL(actualList[i].GetPosition(), expectedList[i].GetPosition());
	}
}

BOOST_AUTO_TEST_SUITE(lexer_test)
	BOOST_AUTO_TEST_CASE(can_identify_keyword)
	{
		std::istringstream str("int");
		Lexer lexer(str);
		lexer.Run();
		auto tokens = lexer.GetTokensList();
		BOOST_CHECK(tokens[0].GetType() == TokenType::KEYWORD);
	}

	BOOST_AUTO_TEST_CASE(can_skip_ignored_separators)
	{
		std::istringstream str("} \n \t \n float\t");
		Lexer lexer(str);
		lexer.Run();

		auto& tokens = lexer.GetTokensList();
		
		CheckLexemeList(tokens,
			{
				Token("}", TokenType::BLOCK_CLOSE_BRACKET, 0, 0),
				Token("float", TokenType::KEYWORD, 2, 1)
			}
		);
	}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(arithmetic_operators_test)
	BOOST_AUTO_TEST_CASE(can_process_operators_separated_by_lexeme)
	{
		std::istringstream str("float a = 123.23 + a * 2 / 12 - 33");
		Lexer lexer(str);
		lexer.Run();

		auto& tokens = lexer.GetTokensList();

		CheckLexemeList(tokens,
			{
				Token("float", TokenType::KEYWORD, 0, 0),
				Token("a", TokenType::IDENTIFIER, 0, 6),
				Token("=", TokenType::ASSIGNMENT, 0, 8),
				Token("123.23", TokenType::FLOAT, 0, 10),
				Token("+", TokenType::ARITHMETIC_OPERATOR, 0, 17),
				Token("a", TokenType::IDENTIFIER, 0, 19),
				Token("*", TokenType::ARITHMETIC_OPERATOR, 0, 21),
				Token("2", TokenType::DECIMAL_NUMBER, 0, 23),
				Token("/", TokenType::ARITHMETIC_OPERATOR, 0, 25),
				Token("12", TokenType::DECIMAL_NUMBER, 0, 27),
				Token("-", TokenType::ARITHMETIC_OPERATOR, 0, 30),
				Token("33", TokenType::DECIMAL_NUMBER, 0, 32),
			}
		);
	}

	BOOST_AUTO_TEST_CASE(can_process_operators_without_separating_lexemas)
	{
		std::istringstream str("float a=123.23+a*2/12-33");
		Lexer lexer(str);
		lexer.Run();

		auto& tokens = lexer.GetTokensList();

		CheckLexemeList(tokens,
			{
				Token("float", TokenType::KEYWORD, 0, 0),
				Token("a", TokenType::IDENTIFIER, 0, 6),
				Token("=", TokenType::ASSIGNMENT, 0, 7),
				Token("123.23", TokenType::FLOAT, 0, 8),
				Token("+", TokenType::ARITHMETIC_OPERATOR, 0, 14),
				Token("a", TokenType::IDENTIFIER, 0, 15),
				Token("*", TokenType::ARITHMETIC_OPERATOR, 0, 16),
				Token("2", TokenType::DECIMAL_NUMBER, 0, 17),
				Token("/", TokenType::ARITHMETIC_OPERATOR, 0, 18),
				Token("12", TokenType::DECIMAL_NUMBER, 0, 19),
				Token("-", TokenType::ARITHMETIC_OPERATOR, 0, 21),
				Token("33", TokenType::DECIMAL_NUMBER, 0, 22),
			}
		);
	}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(relation_oparators_test)
	BOOST_AUTO_TEST_CASE(can_process_operators)
	{
		std::istringstream str("if (a >= 2 || a <= 3 || a!=b || a==3 || a>3 || a<3)");
		Lexer lexer(str);
		lexer.Run();

		auto& tokens = lexer.GetTokensList();

		CheckLexemeList(tokens,
			{
				Token("if", TokenType::KEYWORD, 0, 0),
				Token("(", TokenType::OPEN_BRACKET, 0, 3),
				Token("a", TokenType::IDENTIFIER, 0, 4),
				Token(">=", TokenType::COMPARISION_OPERATOR, 0, 6),
				Token("2", TokenType::DECIMAL_NUMBER, 0, 9),
				Token("||", TokenType::LOGICAL_OPERATOR, 0, 11),

				Token("a", TokenType::IDENTIFIER, 0, 14),
				Token("<=", TokenType::COMPARISION_OPERATOR, 0, 16),
				Token("3", TokenType::DECIMAL_NUMBER, 0, 19),
				Token("||", TokenType::LOGICAL_OPERATOR, 0, 21),
				
				Token("a", TokenType::IDENTIFIER, 0, 24),
				Token("!=", TokenType::COMPARISION_OPERATOR, 0, 25),
				Token("b", TokenType::IDENTIFIER, 0, 27),
				Token("||", TokenType::LOGICAL_OPERATOR, 0, 29),

				Token("a", TokenType::IDENTIFIER, 0, 32),
				Token("==", TokenType::COMPARISION_OPERATOR, 0, 33),
				Token("3", TokenType::DECIMAL_NUMBER, 0, 35),
				Token("||", TokenType::LOGICAL_OPERATOR, 0, 37),

				Token("a", TokenType::IDENTIFIER, 0, 40),
				Token(">", TokenType::COMPARISION_OPERATOR, 0, 41),
				Token("3", TokenType::DECIMAL_NUMBER, 0, 42),
				Token("||", TokenType::LOGICAL_OPERATOR, 0, 44),

				Token("a", TokenType::IDENTIFIER, 0, 47),
				Token("<", TokenType::COMPARISION_OPERATOR, 0, 48),
				Token("3", TokenType::DECIMAL_NUMBER, 0, 49),
				Token(")", TokenType::CLOSE_BRACKET, 0, 50),
			}
		);
	}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(boolean_test)
	BOOST_AUTO_TEST_CASE(can_identify_bool_values)
	{
		std::istringstream str("{if	(true||false)}");
		Lexer lexer(str);
		lexer.Run();

		auto& tokens = lexer.GetTokensList();

		CheckLexemeList(tokens,
			{
				Token("{", TokenType::BLOCK_OPEN_BRACKET, 0, 0),
				Token("if", TokenType::KEYWORD, 0, 1),
				Token("(", TokenType::OPEN_BRACKET, 0, 4),
				Token("true", TokenType::BOOLEAN, 0, 5),
				Token("||", TokenType::LOGICAL_OPERATOR, 0, 9),
				Token("false", TokenType::BOOLEAN, 0, 11),
				Token(")", TokenType::CLOSE_BRACKET, 0, 16),
				Token("}", TokenType::BLOCK_CLOSE_BRACKET, 0, 17),
			}
		);
	}
BOOST_AUTO_TEST_SUITE_END();



BOOST_AUTO_TEST_SUITE(string_type_test)

	BOOST_AUTO_TEST_CASE(can_get_string_between_two_quotes)
	{
		std::istringstream str("int\"some text in string type\"abc");
		Lexer lexer(str);
		lexer.Run();

		CheckLexemeList(lexer.GetTokensList(),
			{
				Token("int", TokenType::KEYWORD, 0, 0),
				Token("some text in string type", TokenType::STRING, 0, 3),
				Token("abc", TokenType::IDENTIFIER, 0, 29),
			}
		);
	}

	BOOST_AUTO_TEST_CASE(should_set_error_type_for_single_quote_in_line)
	{
		std::istringstream str("int\"int");
		Lexer lexer(str);
		lexer.Run();

		CheckLexemeList(lexer.GetTokensList(),
			{
				Token("int", TokenType::KEYWORD, 0, 0),
				Token("\"", TokenType::ERROR, 0, 3),
				Token("int", TokenType::KEYWORD, 0, 4),
			}
		);
	}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE(comments_test)
	BOOST_AUTO_TEST_CASE(should_skip_comment)
	{
		std::istringstream str("int a = 1234//some text float\nreturn");
		Lexer lexer(str);
		lexer.Run();

		CheckLexemeList(lexer.GetTokensList(),
			{
				Token("int", TokenType::KEYWORD, 0, 0),
				Token("a", TokenType::IDENTIFIER, 0, 4),
				Token("=", TokenType::ASSIGNMENT, 0, 6),
				Token("1234", TokenType::DECIMAL_NUMBER, 0, 8),
				Token("return", TokenType::KEYWORD, 1, 0),
			}
		);
	}

	BOOST_AUTO_TEST_CASE(should_skip_multiline_comment)
	{
		std::istringstream str("int a = 1234/*some text float\n float */ /*return\nfloat\nint");
		Lexer lexer(str);
		lexer.Run();

		CheckLexemeList(lexer.GetTokensList(),
			{
				Token("int", TokenType::KEYWORD, 0, 0),
				Token("a", TokenType::IDENTIFIER, 0, 4),
				Token("=", TokenType::ASSIGNMENT, 0, 6),
				Token("1234", TokenType::DECIMAL_NUMBER, 0, 8),
			}
		);
	}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(numbers_test)
	BOOST_AUTO_TEST_SUITE(decimal_numbers_test)
		BOOST_AUTO_TEST_CASE(can_parse_decimal_number) 
		{
			std::istringstream str("int a = 1234");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("1234", TokenType::DECIMAL_NUMBER, 0, 8),
				}
			);
		}

		BOOST_AUTO_TEST_CASE(should_set_error_type_for_number_with_following_letters)
		{
			std::istringstream str("int a = 1234aaa");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("1234aaa", TokenType::ERROR, 0, 8),
				}
			);
		}

		BOOST_AUTO_TEST_CASE(should_set_error_type_for_number_out_of_range)
		{
			std::istringstream str("int a = 99999999999999999999");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("99999999999999999999", TokenType::ERROR, 0, 8),
				}
			);
		}
	BOOST_AUTO_TEST_SUITE_END();
	BOOST_AUTO_TEST_SUITE(hex_numbers_test)
		BOOST_AUTO_TEST_CASE(can_parse_hex_number)
		{
			std::istringstream str("int a = 0xf");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("0xf", TokenType::HEX_NUMBER, 0, 8),
				}
			);
		}

		BOOST_AUTO_TEST_CASE(should_set_error_type_for_number_with_symbols_out_hex)
		{
			std::istringstream str("int a = 0xFggg");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("0xFggg", TokenType::ERROR, 0, 8),
				}
			);
		}

		BOOST_AUTO_TEST_CASE(should_set_error_type_for_number_out_of_range)
		{
			std::istringstream str("int a = 0xfffffffffffffffff");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("0xfffffffffffffffff", TokenType::ERROR, 0, 8),
				}
			);
		}
	BOOST_AUTO_TEST_SUITE_END();

	BOOST_AUTO_TEST_SUITE(binary_numbers_test)
		BOOST_AUTO_TEST_CASE(can_parse_binary_number)
		{
			std::istringstream str("int a = 0b1111");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("0b1111", TokenType::BINARY_NUMBER, 0, 8),
				}
			);
		}

		BOOST_AUTO_TEST_CASE(should_set_error_type_for_number_with_symbols_out_of_range)
		{
			std::istringstream str("int a = 0b123");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("0b123", TokenType::ERROR, 0, 8),
				}
			);
		}

	BOOST_AUTO_TEST_SUITE_END();


	BOOST_AUTO_TEST_SUITE(octal_numbers_test)
		BOOST_AUTO_TEST_CASE(can_parse_octal_number)
		{
			std::istringstream str("int a = 0777");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("0777", TokenType::OCTAL_NUMBER, 0, 8),
				}
			);
		}

		BOOST_AUTO_TEST_CASE(should_set_error_type_for_number_out_of_range)
		{
			std::istringstream str("int a = 077777777777777777777777777");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("077777777777777777777777777", TokenType::ERROR, 0, 8),
				}
			);
		}
	BOOST_AUTO_TEST_SUITE_END();

	BOOST_AUTO_TEST_SUITE(float_numbers_test)
		BOOST_AUTO_TEST_CASE(can_parse_float_number)
		{
			std::istringstream str("int a = 1.23");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("1.23", TokenType::FLOAT, 0, 8),
				}
			);
		}

		BOOST_AUTO_TEST_CASE(can_parse_float_number_with_no_numbers_after_doth)
		{
			std::istringstream str("int a = 1.");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("1.", TokenType::FLOAT, 0, 8),
				}
			);
		}

		BOOST_AUTO_TEST_CASE(can_parse_float_number_in_exp_format)
		{
			std::istringstream str("int a = 1.23e+4 .23e-4 5.e-4");
			Lexer lexer(str);
			lexer.Run();

			CheckLexemeList(lexer.GetTokensList(),
				{
					Token("int", TokenType::KEYWORD, 0, 0),
					Token("a", TokenType::IDENTIFIER, 0, 4),
					Token("=", TokenType::ASSIGNMENT, 0, 6),
					Token("1.23e+4", TokenType::FLOAT, 0, 8),
					Token(".23e-4", TokenType::FLOAT, 0, 16),
					Token("5.e-4", TokenType::FLOAT, 0, 23),
				}
			);
		}
	BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE_END();