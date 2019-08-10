/*
Copyright � Nico Zink
All rights reserved.
*/

#pragma once

// Local includes
#include "json_token.h"

// Project includes

// System Includes
#include <istream>

class JSON_Lexer
{
public:
	JSON_Lexer(std::istream& input);

	JSON_Token get_current_token();

	double get_number();

	std::string get_string();

	JSON_Token read_next();

private:

	void read(std::string str);

	void read_number();

	void read_string();

	void skip_char();

	void skip_whitespace();

private:

	int m_current_char;
	
	JSON_Token m_current_token;

	std::string m_current_string;
	
	std::istream& m_input;
};
