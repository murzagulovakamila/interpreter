#include <stdexcept>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "parser.hpp"
#define MIN_PRECEDENCE 0



Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), offset(0) {}


std::vector<decl> Parser::parse() {
    std::vector<decl> declarations;
    while (!match(TokenType::END)) {
        declarations.push_back(parse_declaration());
        if (auto test = std::dynamic_pointer_cast<FuncDecl>(declarations[0]); test) {
		if (auto tmp = std::dynamic_pointer_cast<BlockStatement>(test->block); tmp) {
			if (auto test1 = std::dynamic_pointer_cast<PrintStatement>(tmp->statements[0]); test1) {
			
			}
    		}
    	}
    }
    return declarations;
}


decl Parser::parse_declaration() {
    if (tokens[offset].type == TokenType::TYPE && tokens[offset + 1].type == TokenType::IDENTIFIER && tokens[offset + 2].lexeme == "(") {
            return parse_func_declaration();
        } else if(tokens[offset].type == TokenType::TYPE && tokens[offset + 1].type == TokenType::IDENTIFIER ) {
            return parse_var_declaration();
        } else {
        throw std::runtime_error("Invalid declaration");
    }
}

decl Parser::parse_var_declaration() {
    std::string type = extract(TokenType::TYPE);
    std::vector<std::pair<std::string, expr>> expressions;
    while (true) {
        std::string name = extract(TokenType::IDENTIFIER);
        expr expression;
        if (match("=")) {
            offset++;
            expression = parse_expression();
        }
        expressions.push_back(std::make_pair(name, expression));
        if (!match(",")) {
            break;
        }
        offset++;
        
    }
    if (!match(";")) {
    throw std::runtime_error("Expected ; after variable declaration");
}

    return std::make_shared<VarDecl>(type, expressions);
}


decl Parser::parse_param_declaration() {
    std::string type = extract(TokenType::TYPE);
    std::string name = extract(TokenType::IDENTIFIER);
    return std::make_shared<ParamDecl>(type, name);
}

decl Parser::parse_func_declaration() {
    std::string type = extract(TokenType::TYPE);
    std::string name = extract(TokenType::IDENTIFIER);
    //std::cout << "token:" << tokens[offset].lexeme << std::endl;
    if (!match("(")) { //
        throw std::runtime_error("Expected (");
    }
    offset++;
    std::vector<decl> parameters;
    while (!match(")")) {
        parameters.push_back(parse_param_declaration());
        if (match(",")) {
            offset++;
        } else if (!match(")")) {
            throw std::runtime_error("Expected , or )");
        }
    }
    offset++;
    st block = parse_block_statement();
    
    return std::make_shared<FuncDecl>(type, name, parameters, block);
}



st Parser::parse_statement(){
		if(match("if")){
			return parse_cond_statement();
		} else if(match("while")){
			return parse_while_statement();
		} else if(match("for")){
			return parse_for_statement();
		} else if(match("break")){
			return parse_break_statement();
		} else if(match("continue")){
			return parse_continue_statement();
		} else if (match("return")){
			return parse_return_statement();
		} else if(match ("{")){
			return parse_block_statement();
		} else if(match(TokenType::TYPE)){
			return parse_decl_statement();
		} else if(match(TokenType::PRINT)){
			return parse_print_state();
		} else{
			return parse_expr_statement();
		}
}

st Parser::parse_block_statement() {
    if (!match("{")) {
        throw std::runtime_error("Expected {");
    }
    offset++;
    std::vector<st> statements;
    while (!match("}")) {
        statements.push_back(parse_statement());
        if(match(";")){
        	offset++;
    	}
    }
    if (!match("}")) {
        throw std::runtime_error("Expected }");
    }
    offset++;
    return std::make_shared<BlockStatement>(statements);
}

st Parser::parse_expr_statement() {
    expr expression = parse_expression();
    offset++;
    return std::make_shared<ExpressionStatement>(expression);
    if (!match(";")) {
        throw std::runtime_error("Expected ; 1 ");
    }
}

st Parser::parse_decl_statement() {
    decl declaration = parse_var_declaration();
    if (!match(";")) {
        throw std::runtime_error("Expected ; 2");
    }
    offset++;
    return std::make_shared<DeclarationStatement>(declaration);
}


st Parser::parse_cond_statement() {
    if (!match("if")) {
        throw std::runtime_error("Expected if");
    }
    offset++;
    if (!match("(")) {
        throw std::runtime_error("Expected (");
    }
    offset++;
    expr condition = parse_expression();
    if (!match(")")) {
        throw std::runtime_error("Expected )");
    }
    offset++;
    st if_block = parse_statement();
    if (match("else")) {
        offset++;
        st else_block = parse_statement();
        return std::make_shared<ConditionalStatement>(condition, if_block, else_block);
    }
    return std::make_shared<ConditionalStatement>(condition, if_block, nullptr);
}

st Parser::parse_while_statement() {
    if (!match("while")) {
        throw std::runtime_error("Expected while");
    }
    offset++;
    if (!match("(")) {
        throw std::runtime_error("Expected (");
    }
    offset++;
    expr condition = parse_expression();
    if (!match(")")) {
        throw std::runtime_error("Expected )");
    }
    offset++;
    st body_statement = parse_statement();
    return std::make_shared<WhileStatement>(condition, body_statement);
}

st Parser::parse_for_statement() {
    if (!match("for")) {
        throw std::runtime_error("Expected for");
    }
    offset++;
    if (!match("(")) {
        throw std::runtime_error("Expected (");
    }
    offset++;
    st init_statement;
    if (match(TokenType::TYPE)) {
        init_statement = parse_decl_statement();
    } else {
        init_statement = parse_expr_statement();
    }
    if (!match(";")) {
        throw std::runtime_error("Expected ;");
    }
    offset++;
    expr condition = parse_expression();
    if (!match(";")) {
        throw std::runtime_error("Expected ; 3 ");
    }
    offset++;
    expr post_expr = parse_expression();
    if (!match(")")) {
        throw std::runtime_error("Expected )");
    }
    offset++;
    st body_statement = parse_statement();
    return std::make_shared<ForStatement>(init_statement, condition, post_expr, body_statement);
}

st Parser::parse_break_statement() {
    if (!match("break")) {
        throw std::runtime_error("Expected break");
    }
    offset++;
    if (!match(";")) {
        throw std::runtime_error("Expected ; 4 ");
    }
    offset++;
    return std::make_shared<BreakStatement>();
}


st Parser::parse_continue_statement() {
    if (!match("continue")) {
        throw std::runtime_error("Expected continue");
    }
    offset++;
    if (!match(";")) {
        throw std::runtime_error("Expected ; 5 ");
    }
    offset++;
    return std::make_shared<ContinueStatement>();
}

st Parser::parse_return_statement() {
    if (!match("return")) {
        throw std::runtime_error("Expected return");
    }
    offset++;
    expr return_expr;
    if (!match(";")) {
        return_expr = parse_expression();
    }
    if (!match(";")) {
        throw std::runtime_error("Expected ; 6 ");
    }
    offset++;
    return std::make_shared<ReturnStatement>(return_expr);
}

st Parser::parse_print_state() {
    extract(TokenType::PRINT);
    if (!match("(")) {
        throw std::runtime_error("Expected ( after print");
    }
    offset++;
    std::vector<expr> args;
    while (!match(")")) {
        args.push_back(parse_expression());
        if (match(",")) {
            offset++;
        } else if (!match(")")) {
            throw std::runtime_error("Expected , or )");
        }
    }
    if (!match(")")) {
        throw std::runtime_error("Expected ) after print");
    }
    offset++;
    return std::make_shared<PrintStatement>(args);
}




expr Parser::parse_expression() {
	return parse_binary_expression(0);
}

expr Parser::parse_binary_expression(int min_precedence) {
    auto lhs = parse_prefix_expression();
    while (true) {
        auto op = tokens[offset].lexeme;
        if (op == ")") break;
        auto it = operators.find(op);
        if (it == operators.end() || it->second < min_precedence) {
            break;
        }
        int precedence = it->second;
        ++offset;
        auto rhs = parse_binary_expression(precedence);
        lhs = std::make_shared<BinaryNode>(op, lhs, rhs);
    }
    return lhs;
}

expr Parser::parse_prefix_expression() {
    if (match("--") || match("++") || match("+") || match("-") || match("!")) {
        std::string op = tokens[offset].lexeme;
        offset++;
        expr operand = parse_prefix_expression();
        return std::make_shared<Prefix>(op, operand);
    } else {
        return parse_postfix_expression();
    }
}

expr Parser::parse_postfix_expression() {
    expr result = parse_primary_expression();
    while (true) {
        if (match("--") || match("++") || match("+") || match("-") || match("!")) {
            std::string op = tokens[offset].lexeme;
            offset++;
            result = std::make_shared<Postfix>(op, result);
        } else {
            break;
        }
    }
    return result;
}

expr Parser::parse_primary_expression() {
    if (match(TokenType::IDENTIFIER) && tokens[offset + 1].type == TokenType::IDENTIFIER && tokens[offset + 2].lexeme == "(") {
        std::string name = extract(TokenType::IDENTIFIER);
     //   if (match("(")) {
            auto expr = parse_function_expression();
            return std::make_shared<IdentifierNode>(name); 
       // } 
    } else if (match(TokenType::IDENTIFIER)){
    	std::string name = extract(TokenType::IDENTIFIER);
            return std::make_shared<IdentifierNode>(name);
        }
    else if (match("(")) {
        return parse_parenthesized_expression();
    } else if (match(TokenType::INT_LITERAL)) {
        std::string lexeme = extract(TokenType::INT_LITERAL);
        int value = std::stoi(lexeme);
        return std::make_shared<LiteralINT>(value);
    } else if (match(TokenType::FLOAT_LITERAL)) {
        std::string lexeme = extract(TokenType::FLOAT_LITERAL);
        float value = std::stof(lexeme);
        return std::make_shared<LiteralFLOAT>(value);
    } else if (match(TokenType::CHAR_LITERAL)) {
        std::string lexeme = extract(TokenType::CHAR_LITERAL);
        char value = lexeme[0];
        return std::make_shared<LiteralCHAR>(value);
    } else if (match(TokenType::STRING_LITERAL)) {
        std::string value = extract(TokenType::STRING_LITERAL);
        return std::make_shared<StringLiteralNode>(value);
    } else if (match(TokenType::BOOL_LITERAL)) {
        std::string lexeme = extract(TokenType::BOOL_LITERAL);
        bool value = (lexeme == "true");
        return std::make_shared<LiteralBOOL>(value);
    } else if (tokens[offset] == "+" || tokens[offset] == "-" || tokens[offset] == "=") {
		++offset;
		return std::make_shared<UnaryNode>(tokens[offset].lexeme, parse_primary_expression());
		} else if (match(";")) {
        offset++;
        return nullptr; 
    } else {
        throw std::runtime_error("Expected primary expression");
    }
}


expr Parser::parse_function_expression() {
    std::string name = extract(TokenType::IDENTIFIER); 
    std::cout << "Parsing function expression: " << name << std::endl;

    if (!match("(")) {
        throw std::runtime_error("Expected ( after function name");
    }
    offset++; 
    std::vector<expr> arguments; 

    while (!match(")")) {
        arguments.push_back(parse_expression());
        std::cout << "Parsed argument: " << arguments.back() << std::endl;

        if (match(",")) {
            offset++; 
        } else if (!match(")")) {
            throw std::runtime_error("Expected , or )");
        }
    }
    
    offset++;
    std::cout << "Finished parsing function expression: " << name << std::endl;
    return std::make_shared<FunctionNode>(name, arguments);
}

expr Parser::parse_parenthesized_expression() {
	if(match("(")){
		offset++;
	}
	auto node = parse_expression();
	if(match(")")){
		offset++;
	}
	return std::make_shared<ParenthesizedNode>(node);
}

bool Parser::match(TokenType expected_type) const {
	return tokens[offset] == expected_type;
}


bool Parser::match(const std::string& name) const {
    return tokens[offset] == name;
}

std::string Parser::extract(TokenType expected_type) {
	if (!match(expected_type)) {
		throw std::runtime_error("Unexpected token " + tokens[offset].lexeme);
	}
	return tokens[offset++].lexeme;
}


const std::unordered_map<std::string, int> Parser::operators = {
	{"++", 0}, {"--", 0},
	{"*", 1}, {"/", 1},
	{"+", 2}, {"-", 2},
	{"==", 3}, {"!=", 3},
	{"&", 4},
	{"^",5},
	{"&&", 6},
	{"||", 7},
	{"=", 8},
	{"+=", 9}, {"-=", 9},
	{"<", 10}, {">", 10},
	{",", 11},  {"(", 11}, {")", 11},
	{"{", 12}, {"}", 12},
};
