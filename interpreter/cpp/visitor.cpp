#include <iostream>
#include "visitor.hpp"
#include <cmath>
#include <stdexcept>
#include "symbol.hpp"

void Printer::print(const node& root) {
    root->accept(*this);
    std::cout << std::endl;
}

void visit(DeclarationNode&)  {
    std::cout << "DeclarationNode";
}

void visit(StatementNode& )  {
    std::cout << "StatementNode";
} 

void visit(ExpressionNode& )  {
    std::cout << "ExpressionNode";
}

void visit(VarDecl& root)  {
	
    std::cout << "VarDecl: " << root.type << " of type " << root.type;
}

void visit(ParamDecl& root)  {
    std::cout << "ParamDecl: " << root.name << " of type " << root.type;
}


void visit(LiteralNode& )  {
        std::cout << "LiteralNode";
    }

void visit(LiteralINT& root)  {
    std::cout << root.value;
}

void visit(LiteralFLOAT& root)  {
    std::cout << root.value;
}

void visit(LiteralCHAR& root)  {
    std::cout << "'" << root.value << "'";
}

void visit(LiteralBOOL& root)  {
    std::cout << (root.value ? "true" : "false");
}

void visit(StringLiteralNode& root)  {
    std::cout << "\"" << root.value << "\"";
}

void visit(ContinueStatement& )  {
    std::cout << "continue";
}

void visit(BreakStatement& )  {
    std::cout << "break";
}

void visit(JumpStatement& )  {
    std::cout << "jump";
}

void visit(PrintStatement&) {
    std::cout << "print(";
}
void visit(BinaryNode& ){
	std::cout << "BinaryNode";
}
 
void visit(UnaryNode& ){
	std::cout << "UnaryNode";
}
 
void visit(FunctionNode& ){
	std::cout << "FunctionNode";
}
    
void visit(IdentifierNode& ){
	std::cout << "IdentifierNode";
}
 
void visit(ParenthesizedNode&){
	std::cout << "void visit(ParenthesizedNode& root){";
}
 
void visit(ExpressionStatement&){
	std::cout << "ExpressionStatement";
}

void visit(BlockStatement&){
	std::cout << "BlockStatement";
}
	
void visit(ForStatement&){
	std::cout << "ForStatement";
}

void visit(DeclarationStatement&){
	std::cout << "DeclarationStatement";
}

void visit(WhileStatement&){
	std::cout << "WhileStatement";
}

void visit(LoopStatement&){
	std::cout << "LoopStatement";
}

void visit(ConditionalStatement&){
	std::cout << "ConditionalStatement";
}

void visit(ReturnStatement&){
	std::cout << "ReturnStatement";
}

void visit(FuncDecl&){
	std::cout << "FuncDecl";
}

void visit(Prefix&){
	std::cout << "Prefix";
}

void visit(Postfix&){
	std::cout << "Postfix";
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Printer::visit(ParamDecl& node) {
    std::cout << "ParamDecl: " << node.name << std::endl;
}

void Printer::visit(VarDecl& node) {
    std::cout << "VarDecl: " << node.name << " of type " << node.type << std::endl;
}

void Printer::visit(StatementNode&) {
    std::cout << "StatementNode" << std::endl;
}

void Printer::visit(DeclarationNode&) {
    std::cout << "DeclarationNode" << std::endl;
}

void Printer::visit(BinaryNode& node) {
    std::cout << "BinaryNode with operator " << node.op << std::endl;
    node.lhs->accept(*this);
    node.rhs->accept(*this);
}

void Printer::visit(UnaryNode& node) {
    std::cout << "UnaryNode with operator " << node.op << std::endl;
    node.branch->accept(*this);
}

void Printer::visit(FunctionNode& node) {
    std::cout << "FunctionNode: " << node.name << std::endl;
    for (const auto& branch : node.branches) {
        branch->accept(*this);
    }
}

void Printer::visit(IdentifierNode& node) {
    std::cout << "IdentifierNode: " << node.name << std::endl;
}


void Printer::visit(ParenthesizedNode& node) {
    std::cout << "ParenthesizedNode" << std::endl;
    node.expression->accept(*this);
}

void Printer::visit(BlockStatement& node) {
    std::cout << "BlockStatement" << std::endl;
    for (auto& statement : node.statements) {
        statement->accept(*this);
    }
}

void Printer::visit(ExpressionNode& node) {
    std::cout << "ExpressionNode" << std::endl;
    if (auto* binaryNode = dynamic_cast<BinaryNode*>(&node)) {
        visit(*binaryNode);
    } else if (auto* unaryNode = dynamic_cast<UnaryNode*>(&node)) {
        visit(*unaryNode);
    } else if (auto* identifierNode = dynamic_cast<IdentifierNode*>(&node)) {
        visit(*identifierNode);
    } else if (auto* parenthesizedNode = dynamic_cast<ParenthesizedNode*>(&node)) {
        visit(*parenthesizedNode);
    } else {
        std::cerr << "Unknown expression type" << std::endl;
    }
}

void Printer::visit(ReturnStatement& node) {
    std::cout << "ReturnStatement" << std::endl;
    if (node.return_value) {
        node.return_value->accept(*this);
    }
}

void Printer::visit(ContinueStatement&) {
    std::cout << "ContinueStatement" << std::endl;
}

void Printer::visit(BreakStatement&) {
    std::cout << "BreakStatement" << std::endl;
}

void Printer::visit(JumpStatement&) {
    std::cout << "JumpStatement" << std::endl;
}

void Printer::visit(ForStatement& node) {
    std::cout << "ForStatement" << std::endl;
    if (node.init) {
        node.init->accept(*this);
    }
    if (node.condition) {
        node.condition->accept(*this);
    }
    if (node.increment) {
        node.increment->accept(*this);
    }
    if (node.body) {
        node.body->accept(*this);
    }
}

void Printer::visit(DeclarationStatement& node) {
    std::cout << "DeclarationStatement" << std::endl;
    node.declaration->accept(*this);
}

void Printer::visit(WhileStatement& node) {
    std::cout << "WhileStatement" << std::endl;
    node.condition->accept(*this);
    node.body->accept(*this);
}

void Printer::visit(NumberNode&) {
}

void Printer::visit(LoopStatement&) {
    std::cout << "LoopStatement" << std::endl;
}

void Printer::visit(ConditionalStatement& node) {
    std::cout << "ConditionalStatement" << std::endl;
    node.condition->accept(*this);
    if (node.if_statement) {
        node.if_statement->accept(*this);
    }
    if (node.else_statement) {
        node.else_statement->accept(*this);
    }
}

void Printer::visit(ExpressionStatement& node) {
    std::cout << "ExpressionStatement" << std::endl;
    node.Expr->accept(*this);
}

void Printer::visit(PrintStatement& node) {
    std::cout << "print(";
    for (auto expr : node.args) {
        expr->accept(*this);
        if (expr == *((node.args.end()--)))
            std::cout << " ,";
    }
    std::cout << ")\n";
}

void Printer::visit(FuncDecl&) {
}

void Printer::visit(Prefix&) {
    std::cout << "Prefix" << std::endl;
}

void Printer::visit(Postfix&) {
    std::cout << "Postfix" << std::endl;
}

void Printer::visit(LiteralNode&) {
    std::cout << "LiteralNode" << std::endl;
}

void Printer::visit(LiteralINT& node) {
    std::cout << "LiteralINT: " << node.value << std::endl;
}

void Printer::visit(LiteralFLOAT& node) {
    std::cout << "LiteralFLOAT: " << node.value << std::endl;
}

void Printer::visit(LiteralCHAR& node) {
    std::cout << "LiteralCHAR: " << node.value << std::endl;
}

void Printer::visit(LiteralBOOL& node) {
    std::cout << "LiteralBOOL: " << node.value << std::endl;
}

void Printer::visit(StringLiteralNode& node) {
    std::cout << "StringLiteralNode: " << node.value << std::endl;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


std::shared_ptr<Scope> Analyzer::analyze(const node& root) {
    root->accept(*this);
    return scopeManager.scopes.top();
}

void Analyzer::visit(VarDecl& root) {
    auto currentScope = scopeManager.scopes.top();

    if (currentScope->lookup(root.name)) {
        throw std::runtime_error("Variable " + root.name + " already declared.");
    }

    std::shared_ptr<Type> varType;
    if (root.type == "int") {
        varType = std::make_shared<IntType>();
    } else if (root.type == "float") {
        varType = std::make_shared<FloatType>();
    } else if (root.type == "char") {
        varType = std::make_shared<CharType>();
    } else if (root.type == "bool") {
        varType = std::make_shared<BoolType>();
    } else if (root.type == "string") {
        varType = std::make_shared<StringType>();
    } else {
        throw std::runtime_error("Unknown type for variable " + root.name);
    }

    auto variable = std::make_shared<Variable>();
    variable->type = varType;

    currentScope->table[root.name] = variable;
}


void Analyzer::visit(FuncDecl& root) {
    if (scopeManager.scopes.top()->lookup(root.name)) {
        throw std::runtime_error("Function with name '" + root.name + "' is already declared.");
    }
    auto function = std::make_shared<Function>();
    function->returnType = root.returnType;
    for (const auto& param : root.parameters) {
        auto variable = std::make_shared<Variable>();
        variable->type = std::any_cast<std::shared_ptr<Type>>(param->type);
			for (const auto& param : root.parameters) {
		auto variable = std::make_shared<Variable>();
		variable->type = std::any_cast<std::shared_ptr<Type>>(param->type);
		if (auto varDecl = dynamic_cast<VarDecl*>(param.get())) {
			if (!varDecl->expressions.empty()) {
			    variable->value = varDecl->expressions[0].second->value;
			}
		} 
		function->arguments.push_back({param->name, variable});
	}
	function->arguments.push_back(std::pair<std::string, std::shared_ptr<Variable>>(param->name, variable));
    }
    scopeManager.scopes.top()->add(root.name, function);
    scopeManager.enterScope();
    scopeManager.exitScope();
}

void Analyzer::visit(DeclarationNode&)  {
}

void Analyzer::visit(ConditionalStatement& root) {
    if (root.condition) {
        root.condition->accept(*this);
    }
    if (root.if_statement) {
        scopeManager.enterScope();
        root.if_statement->accept(*this);
        scopeManager.exitScope();
    }
    if (root.else_statement) {
        scopeManager.enterScope();
        root.else_statement->accept(*this);
        scopeManager.exitScope();
    }
}

void Analyzer::visit(WhileStatement& root) {
    
	if (!root.condition) {
        throw std::runtime_error("While loop condition is missing.");
    }
    if (std::dynamic_pointer_cast<BoolType>(root.condition->value->type) == nullptr) {
        throw std::runtime_error("While loop condition must be a boolean expression.");
    }
    if (!root.while_statement) {
        throw std::runtime_error("While loop body is missing.");
    }
    scopeManager.enterScope();
    root.while_statement->accept(*this);
    scopeManager.exitScope();
} 

void Analyzer::visit(DeclarationStatement& root) {
    root.declaration->accept(*this);
}

void Analyzer::visit(ForStatement& root) {
    scopeManager.enterScope();
    root.init->accept(*this);
    root.condition->accept(*this);
    root.increment->accept(*this);
    root.body->accept(*this);
    scopeManager.exitScope();
}

void Analyzer::visit(StatementNode&) {
}

void Analyzer::visit(ContinueStatement&) {
    if (!insideLoop) {
        throw std::runtime_error("Continue statement must be inside a loop.");
    }
    cont = true;
}

void Analyzer::visit(BreakStatement&) {
    if (!insideLoop) {
        throw std::runtime_error("Break statement must be inside a loop.");
    }
    brk = true;
}

void Analyzer::visit(ReturnStatement& root) {
    root.expr->accept(*this);
 	insideLoop = true;
 	func_res = result;   
}

void Analyzer::visit(BlockStatement& root) {
    scopeManager.enterScope();
    for (auto& statement : root.statements) {
        statement->accept(*this);
    }
    scopeManager.exitScope();
}

void Analyzer::visit(PrintStatement& root){
	std::shared_ptr<Variable> current_id;
	for (auto it = root.args.begin(); it != root.args.end(); it++) {
		(*it)->accept(*this);
	}
}

void Analyzer::visit(ExpressionNode& root) {
    root.expression->accept(*this);
}

void Analyzer::visit(BinaryNode& root) {
    if (root.op == "=") {
        if (!root.lhs || dynamic_cast<IdentifierNode*>(root.lhs.get()) == nullptr) {
            throw std::runtime_error("Left-hand side of assignment must be an identifier.");
        }
        root.rhs->accept(*this);
    } else {
        root.rhs->accept(*this);
        auto right_res = result;
        root.lhs->accept(*this);
        auto left_res = result;
        
        result = binary_op.at(root.op)(left_res, right_res);
    }
}

void Analyzer::visit(UnaryNode& root) {
    if (unary_op.find(root.op) == unary_op.end()) {
        throw std::runtime_error("Unsupported unary operation: " + root.op);
    }
    try {
        result = unary_op.at(root.op)(std::any());
    } catch (const std::exception& e) {
        throw std::runtime_error("Error in unary operation: " + std::string(e.what()));
    }
}



void Analyzer::visit(IdentifierNode& root) {
    auto currentScope = scopeManager.scopes.top();
    auto search = currentScope->table.find(root.name);
    while (search == currentScope->table.end()) {
        if (currentScope->parent == nullptr) {
            throw std::runtime_error("Variable " + root.name + " is not declared.");
        }
        currentScope = currentScope->parent;
        search = currentScope->table.find(root.name);
    }

    vars.insert(root.name);
}

void Analyzer::visit(FunctionNode& root){
    if (custom_funcs.contains(root.name)) {
            if (root.branches.size() != (std::dynamic_pointer_cast<FunctionNode>(custom_funcs.at(root.name)))->branches.size()) {
                throw std::runtime_error("Redefined Function");
            }
    } else {
        custom_funcs[root.name] = std::make_shared<FunctionNode>(root);
        for (auto& branch : root.branches) {
            branch->accept(*this);
        }
    }
}

void Analyzer::visit(ParenthesizedNode& root){
    root.expr->accept(*this);
}
	
void Analyzer::visit(Prefix& root) {
    if (root.operation != "not" || root.operation != "-") {
        throw std::runtime_error("Invalid operation");
    } 
    root.branch->accept(*this);
}

void Analyzer::visit(Postfix& ) {
}


void Analyzer::visit(ExpressionStatement& root){
	root.Expr->accept(*this);
}

void Analyzer::visit(LoopStatement&){
}

void Analyzer::visit(JumpStatement&){
}

void Analyzer::visit(ParamDecl&){
}

void Analyzer::visit(LiteralNode&){
}
void Analyzer::visit(LiteralINT&) {
}
void Analyzer::visit(LiteralFLOAT&){
}
void Analyzer::visit(LiteralCHAR&){
}
void Analyzer::visit(NumberNode&){
}
void Analyzer::visit(LiteralBOOL&){
}
void Analyzer::visit(StringLiteralNode&){
}
