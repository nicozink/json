/*
Copyright � Nico Zink
All rights reserved.
*/

#include "json_lexer.h"

// System includes
#include <sstream>

JSON_Lexer::JSON_Lexer(std::istream& input)
	: m_input(input)
{
	skip_char();
}

JSON_Token JSON_Lexer::get_current_token()
{
	return m_current_token;
}

float JSON_Lexer::get_number()
{
	return stof(m_current_string);
}

std::string JSON_Lexer::get_string()
{
	return m_current_string;
}

JSON_Token JSON_Lexer::read_next()
{
	skip_whitespace();

	switch (m_current_char)
	{
		case '{':
			m_current_token = JSON_Token::begin_object;
			read("{");
			break;

		case '}':
			m_current_token = JSON_Token::end_object;
			read("}");
			break;
			
		case '[':
			m_current_token = JSON_Token::begin_array;
			read("[");
			break;
		
		case ']':
			m_current_token = JSON_Token::end_array;
			read("]");
			break;
		
		case 'n':
			m_current_token = JSON_Token::literal_null;
			read("null");
			break;

		case 't':
			m_current_token = JSON_Token::literal_true;
			read("true");
			break;
		
		case 'f':
			m_current_token = JSON_Token::literal_false;
			read("false");
			break;

		case ':':
			m_current_token = JSON_Token::separator_colon;
			read(":");
			break;

		case ',':
			m_current_token = JSON_Token::separator_comma;
			read(",");
			break;

		case '"':
			m_current_token = JSON_Token::string;
			read_string();
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':
			m_current_token = JSON_Token::number;
			read_number();
			break;
		
		case 0:
			m_current_token = JSON_Token::end_of_input;
			m_current_string = "";
			break;
	}

	return m_current_token;
}

void JSON_Lexer::read(std::string str)
{
	for (auto c : str)
	{
		if (m_current_char == c)
		{
			skip_char();
		}
		else
		{
			throw "Unexpected character in token.";
		}
	}

	m_current_string = str;
}

void JSON_Lexer::read_number()
{
	std::string result = "";

	while (
		m_current_char == '0' ||
		m_current_char == '1' ||
		m_current_char == '2' ||
		m_current_char == '3' ||
		m_current_char == '4' ||
		m_current_char == '5' ||
		m_current_char == '6' ||
		m_current_char == '7' ||
		m_current_char == '8' ||
		m_current_char == '9' ||
		m_current_char == '-' ||
		m_current_char == '.')
	{
		result += m_current_char;

		skip_char();
	}

	m_current_string = result;
}

void JSON_Lexer::read_string()
{
	if (m_current_char != '\"')
	{
		throw "String token expected.";
	}

	std::string result = "";

	skip_char();

	while (m_current_char != '\"')
	{
		if (m_current_char == 0)
		{
			throw "Unexpected end of input.";
		}

		result += m_current_char;

		skip_char();
	}

	skip_char();

	m_current_string = result;
}

void JSON_Lexer::skip_char()
{
	m_input.get(m_current_char);
	
	if (m_input.eof())
	{
		m_current_char = 0;
	}
}

void JSON_Lexer::skip_whitespace()
{
	while (m_current_char == ' ' || m_current_char == '\t' || m_current_char == '\n' || m_current_char == '\r')
	{
		skip_char();
	}
}
