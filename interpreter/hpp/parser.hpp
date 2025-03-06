//#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include "token.hpp"
#include "node.hpp"

class Parser{
public: 
	Parser(const std::vector<Token>&);
	std::vector<decl> parse();
private:
	decl parse_declaration();
	decl parse_var_declaration();
	decl parse_param_declaration();
	decl parse_func_declaration(); 

	st parse_statement();
	st parse_block_statement();
	st parse_expr_statement();
	st parse_decl_statement();
	st parse_cond_statement();
	st parse_while_statement();
	st parse_for_statement();
	st parse_break_statement();
	st parse_continue_statement();
	st parse_return_statement();
	st parse_print_state();

	expr parse_expression();
	expr parse_binary_expression(int);
	expr parse_prefix_expression();
    expr parse_postfix_expression();
    expr parse_primary_expression();
	expr parse_function_expression();
	expr parse_parenthesized_expression();

	std::size_t offset;
    std::vector<Token> tokens;
    bool match(TokenType expected_type) const;
    bool match(const std::string&) const;
    std::string extract(TokenType);
    static const std::unordered_map<std::string, int> operators;
	std::string lexeme;
	TokenType current_token();
    
};
