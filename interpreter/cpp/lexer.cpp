#include "lexer.hpp"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& input) : input(input), offset(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (offset < input.size()) {
        if (std::isspace(input[offset])) {
            ++offset;
        } else if (std::isdigit(input[offset]) || input[offset] == '.') {
            tokens.push_back(extract_number());
        } else if (std::isalpha(input[offset]) || input[offset] == '_') {
                tokens.push_back(extract_identifier());
        } else if (metachars.contains(input[offset])) {
			tokens.push_back(extract_operator());
		} else if (input[offset] == '\"') {
            tokens.push_back(extract_string_literal());
        } else {
            throw std::runtime_error("Unknown symbol");
        }
    }
    tokens.push_back(Token{TokenType::END, ""});
    return tokens;
}


Token Lexer::extract_identifier() {
    std::size_t i = 0;
    for (; std::isalnum(input[offset + i]) || input[offset + i] == '_'; ++i);
    std::string identifier(input, offset, i);
    offset += i;

    TokenType type = (types.find(identifier) != types.end()) ? TokenType::TYPE : TokenType::IDENTIFIER;
    Token token{type, identifier};
    if (identifier == "print") {
    	token.type = TokenType::PRINT;
    }
    return token;
}


Token Lexer::extract_number() {
	std::size_t i = 0;
	for(; std::isdigit(input[offset + i]); ++i);
	auto int_len = i;
	auto type = TokenType::INT_LITERAL;
	if (input[offset + i] == '.') {
		type = TokenType::FLOAT_LITERAL;
		++i;
		auto j = i;
		for (; std::isdigit(input[offset + i]); ++i);
		if (i - j == 0 && int_len == 0) {
			throw std::runtime_error("Missing int and float part of number");
		}
	}
	Token token{type, std::string(input, offset, i)};
	offset += i;
	return token;
}


Token Lexer::extract_operator() {
	size_t i = 0;
	std::string op;
	for (; metachars.contains(input[offset + i]); ++i) {
		op += input[offset + i];
		if (!operators.contains(op)) {
			if (i == 0) {
				throw std::runtime_error("Invalid operator");
			}
			op.pop_back();
			break;
		}
	}
	Token token{TokenType::SPECIALS, op};
	offset += i;
	return token;
}


Token Lexer::extract_string_literal() {
    if (input[offset] != '\"') {
        throw std::runtime_error("Invalid string literal");
    }
    std::size_t i = 1;
    while (offset + i < input.size() && input[offset + i] != '\"') {
        ++i;
    }
    if (offset + i >= input.size()) {
        throw std::runtime_error("Unterminated string literal");
    }
    Token token{TokenType::STRING_LITERAL, std::string(input, offset + 1, i - 1)};
    offset += i + 1;
    return token;
}
Token Lexer::extract_bool_literal() {
    std::string boolLiteral;
    boolLiteral += input[offset++]; 
    while (offset < input.size() && std::isalpha(input[offset])) {
        boolLiteral += input[offset++];
    }
    if (boolLiteral == "true" || boolLiteral == "false") {
        return Token{TokenType::BOOL_LITERAL, boolLiteral};
    } else {
        throw std::runtime_error("Invalid boolean literal");
    }
}

const std::string Lexer::metachars = "-*/^+-><(){},;&=!";
const std::unordered_set<std::string> Lexer::operators = {"+", "-", "*", "/", "^", "&&", "==", "!=", "++", "--", "=", "||", ";", "&", ",", "<", ">", "(", ")", "{", "}"};
const std::unordered_set<std::string> Lexer::keywords = {"return", "if", "else", "while"};
const std::unordered_set<std::string> Lexer::types = {"int", "float", "char"};


