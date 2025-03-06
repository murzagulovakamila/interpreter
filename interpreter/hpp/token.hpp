#pragma once
#include <string>
#include <iostream>



enum class TokenType{
	TYPE, IDENTIFIER, KEYWORD, SPECIALS, INT_LITERAL, FLOAT_LITERAL, CHAR_LITERAL, STRING_LITERAL, BOOL_LITERAL, PRINT, DEFAULT, END
	};

const std::string metachars = "+-*/^-><(){},;|&=!";

struct Token {
    TokenType type;
    std::string lexeme;
	TokenType current_token();
    bool operator==(TokenType other_type) const {
    	return type == other_type;
    }
    bool operator==(const std::string& other_lexeme) const {
    	return lexeme == other_lexeme;
    }
    void print() {
	std::cout << "TYPE " << static_cast<int>(type) << " " << lexeme << std::endl;
    }
};
