#include "visitor.hpp"
#include "node.hpp"
#include "value.hpp"
#include "symbol.hpp"
//#include "operation.hpp"


void Executor::execute(decl& root) {
    if (root) {
        root->accept(*this);
    }
}

void Executor::visit(ParamDecl&) {
	std::cout << "param: " << std::endl;
}

void Executor::visit(VarDecl& node) {
    std::shared_ptr<Scope> currentScope = scopeManager.scopes.top();
    std::shared_ptr<Variable> var = std::make_shared<Variable>();
    if (node.type == "int") {
        var->type = std::make_shared<IntType>();
    } else if (node.type == "float") {
        var->type = std::make_shared<FloatType>();
    } else if (node.type == "char") {
        var->type = std::make_shared<CharType>();
    } else if (node.type == "bool") {
        var->type = std::make_shared<BoolType>();
    } else if (node.type == "string") {
        var->type = std::make_shared<StringType>();
    } else {
    std::cout << "EXECUTE: " << std::endl;
        throw std::runtime_error("Unknown type: " + node.type);
    }
    try {
        currentScope->add(node.name, var);
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("Redeclaration of symbol " + node.name + ".");
    }
}

void Executor::visit(StatementNode& node) {
    if (auto* varDeclNode = dynamic_cast<VarDecl*>(&node)) {
        visit(*varDeclNode);
    } else if (auto* funcDeclNode = dynamic_cast<FuncDecl*>(&node)) {
        visit(*funcDeclNode);
    } else if (auto* exprStmtNode = dynamic_cast<ExpressionStatement*>(&node)) {
        visit(*exprStmtNode);
    } else if (auto* blockStmtNode = dynamic_cast<BlockStatement*>(&node)) {
        visit(*blockStmtNode);
    } else if (auto* condStmtNode = dynamic_cast<ConditionalStatement*>(&node)) {
        visit(*condStmtNode);
    } else if (auto* whileStmtNode = dynamic_cast<WhileStatement*>(&node)) {
        visit(*whileStmtNode);
    } else if (auto* forStmtNode = dynamic_cast<ForStatement*>(&node)) {
        visit(*forStmtNode);
    } else if (auto* returnStmtNode = dynamic_cast<ReturnStatement*>(&node)) {
        visit(*returnStmtNode);
    } else if (auto* breakStmtNode = dynamic_cast<BreakStatement*>(&node)) {
        visit(*breakStmtNode);
    } else if (auto* continueStmtNode = dynamic_cast<ContinueStatement*>(&node)) {
        visit(*continueStmtNode);
    } else {
        throw std::runtime_error("Unknown statement type");
    }
}

void Executor::visit(DeclarationNode& node) {
    if (auto* varDeclNode = dynamic_cast<VarDecl*>(&node)) {
        visit(*varDeclNode);
    } else if (auto* funcDeclNode = dynamic_cast<FuncDecl*>(&node)) {
        visit(*funcDeclNode);
    } else {
        throw std::runtime_error("Unknown declaration type");
    }
}

void Executor::visit(BinaryNode& node) {
    node.lhs->accept(*this);
    node.rhs->accept(*this);

    std::any result;
    if (binary_op.find(node.op) != binary_op.end()) {
        try {
            result = binary_op.at(node.op)(
                std::any_cast<std::shared_ptr<Rvalue>>(node.lhs->value)->type,
                std::any_cast<std::shared_ptr<Rvalue>>(node.rhs->value)->type
            );
        } catch (const std::bad_any_cast& e) {
            std::cerr << "Bad1: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Unknown operator: " << node.op << std::endl;
    }

    std::cout << "Result: " << any_to_string(result) << std::endl;
}

void Executor::visit(UnaryNode& node) {
    node.branch->accept(*this);

    std::any result;
    if (unary_op.find(node.op) != unary_op.end()) {
        try {
            result = unary_op.at(node.op)(
                std::any_cast<std::shared_ptr<Rvalue>>(node.branch->value)->type
            );
        } catch (const std::bad_any_cast& e) {
            std::cerr << "Bad2 : " << e.what() << std::endl;
        }
    } else {
        std::cout << "Unknown operator: " << node.op << std::endl;
    }

    std::cout << "Result: " << any_to_string(result) << std::endl;
}

void Executor::visit(FunctionNode& node) {
    scopeManager.enterScope();
    for (const auto& branch : node.branches) {
        branch->accept(*this);
    }
    scopeManager.exitScope();
}

void Executor::visit(IdentifierNode& root) {
    current_id = get_variable(root.name);  
}

std::shared_ptr<Variable> Executor::get_variable(std::string& name) {
    if (lookup(name)) {
        return scopeManager.scopes.top()->get_variable(name);
    } else {
        auto tmp = std::make_shared<Variable>();
        scopeManager.scopes.top()->add(name, tmp);
        return scopeManager.scopes.top()->get_variable(name);
    }
}

void Executor::visit(LoopStatement&) {
}


bool Executor::lookup(std::string& name) {
    return scopeManager.scopes.top()->lookup(name);
}
void Executor::visit(ParenthesizedNode& node) {
    node.expression->accept(*this);
}

void Executor::visit(BlockStatement& node) {
    scopeManager.enterScope();

    for (auto& statement : node.statements) {
        statement->accept(*this);
        if (continue_flag || returnFlag || break_flag) {
        	break;
        }
    }
    scopeManager.exitScope();
}

void Executor::visit(ExpressionNode& node) {
    if (auto* binaryNode = dynamic_cast<BinaryNode*>(&node)) {
        visit(*binaryNode);
    } else if (auto* unaryNode = dynamic_cast<UnaryNode*>(&node)) {
        visit(*unaryNode);
    } else if (auto* identifierNode = dynamic_cast<IdentifierNode*>(&node)) {
        visit(*identifierNode);
    } else if (auto* parenthesizedNode = dynamic_cast<ParenthesizedNode*>(&node)) {
        visit(*parenthesizedNode);
    } else {
        throw std::runtime_error("Unknown expression type");
    }
}

void Executor::visit(ReturnStatement& node) {
    if (node.return_value) {
        node.return_value->accept(*this);
    }
    returnFlag = true;
    //throw std::runtime_error("ReturnException");
}

void Executor::visit(ContinueStatement&) {
	continue_flag = 1;
    //throw std::runtime_error("Continue Exception");
}

void Executor::visit(BreakStatement&) {
	break_flag = 1;
}

void Executor::visit(JumpStatement&) {
	std::cout << "jump: " << std::endl;
}

void Executor::visit(ForStatement& node) {
	std::cout << "for: " << std::endl;
    if (node.init) {
        node.init->accept(*this);
    }

    while (true) {
        if (node.condition) {
            node.condition->accept(*this);
            bool conditionValue = std::any_cast<bool>(node.condition->value);
            if (!conditionValue) {
                break;
            }
        }

        try {
            if (node.body) {
                node.body->accept(*this);
            }
        } catch (const std::runtime_error& e) {
            if (std::string(e.what()) == "Break Exception") {
                break;
            } else if (std::string(e.what()) == "Continue Exception") {
                continue;
            }
        }

        if (node.increment) {
            node.increment->accept(*this);
        }
    }
}

void Executor::visit(DeclarationStatement& node) {
    node.declaration->accept(*this);
}

void Executor::visit(WhileStatement& node) {
    while (true) {
        node.condition->accept(*this);
        bool conditionValue;
        if (result.type() == typeid(int)) {
        	conditionValue = std::any_cast<int>(result) > 0 ? true : false;
        } else if (result.type() == typeid(double)) {
        	conditionValue = std::any_cast<double>(result) > 0 ? true : false;
        } else if (result.type() == typeid(bool)) {
		conditionValue = std::any_cast<bool>(result);
	}

        if (!conditionValue) {
            break;
        }
        
        if (returnFlag) break;
        else if (continue_flag) {
        	continue_flag = 0;
        	continue;
        } else if (break_flag) {
        	break_flag = 0;
        	break;
        }
        node.while_statement->accept(*this);
    }
    break_flag = continue_flag = 0;
}
bool Executor::check(std::any var) {
    if (var.type() == typeid(std::string)) {
        return !(std::any_cast<std::string>(var).empty()) ? true : false;
    } else if (var.type() == typeid(int)) {
        return std::any_cast<int>(var) ? true : false;
    } else if (var.type() == typeid(double)) {
        return std::any_cast<double>(var) ? true : false;
    } else if (var.type() == typeid(bool)) {
        return std::any_cast<bool>(var);
    } else {
        return true;
    }
}


void Executor::visit(ConditionalStatement& node) {
    node.condition->accept(*this);
    bool conditionValue = std::any_cast<bool>(node.condition->value);
    if (conditionValue) {
        if (node.if_statement) {
            node.if_statement->accept(*this);
        }
    } else {
        if (node.else_statement) {
            node.else_statement->accept(*this);
        }
    }
}

void Executor::visit(ExpressionStatement& node) {
    node.Expr->accept(*this);
}

void Executor::visit(PrintStatement& root) {
    for (auto it = root.args.begin(); it != root.args.end(); it++){
        (*it)->accept(*this);
        if (result.type() == typeid(std::string)) {
            std::cout << std::any_cast<std::string>(result) << std::endl;
        } else if (result.type() == typeid(int)) {
            std::cout << std::any_cast<int>(result) << std::endl;
        } else if (result.type() == typeid(double)) {
            std::cout << std::any_cast<double>(result) << std::endl;
        } else if (result.type() == typeid(bool)) {
            std::cout << std::any_cast<bool>(result) << std::endl;
        }   
    }
}


void Executor::visit(FuncDecl& root) {
	if (root.name == "main") {
		root.block->accept(*this);
	}
}

void Executor::visit(Prefix& root) {
	std::cout << "prefix: " << std::endl;
	root.branch->accept(*this);
   // current_id->value = unary_op.at(root.op)(current_id);
}

void Executor::visit(Postfix&) {
	std::cout << "postfix: " << std::endl;
}


void Executor::visit(LiteralNode&) {
}

void Executor::visit(NumberNode& node) {
    result = node.value;
} 

void Executor::visit(LiteralINT& node){
    result = node.value;
}

void Executor::visit(LiteralFLOAT& node) {
    result = node.value;
}

void Executor::visit(LiteralCHAR& node) {
    std::cout << "LiteralCHAR: " << node.value << std::endl;
    std::shared_ptr<Variable> var = std::make_shared<Variable>();
    var->type = std::make_shared<CharType>();
    var->value = node.value;
    scopeManager.scopes.top()->add("literal_char", var);
    try {
        std::cout << "LiteralCHAR value: " << std::any_cast<char>(var->value) << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Error casting value: " << e.what() << std::endl;
    }
}

void Executor::visit(LiteralBOOL& node) {
    std::cout << "LiteralBOOL: " << node.value << std::endl;
    std::shared_ptr<Variable> var = std::make_shared<Variable>();
    var->type = std::make_shared<BoolType>();
    var->value = node.value;
    scopeManager.scopes.top()->add("literal_bool", var);
    try {
        std::cout << "LiteralBOOL value: " << std::any_cast<bool>(var->value) << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Error casting value: " << e.what() << std::endl;
    }
}

void Executor::visit(StringLiteralNode&  node) {
	std::cout << "String Literal: " << node.value << std::endl;
	result = node.value;
}


std::string Executor::any_to_string(const std::any& el) {
        try {
            return std::to_string(std::any_cast<int>(el));
        }
        catch (...) {
            try {
                return std::to_string(std::any_cast<float>(el));
            }
            catch (...) {
                try {
                    return std::to_string(std::any_cast<bool>(el));
                }
                catch (...) {
                    try {
                        return std::any_cast<std::string>(el);
                    }
                    catch (...) {
                        return "unknown";
                    }
                }
            }
        }
    }
    
    


const std::unordered_map<std::string, std::function<std::any(const std::any&)>> Executor::unary_op = {
	
     {"+", [](const std::any& x) -> std::any {
        return x;
    }},
    {"-", [](const std::any& operand) -> std::any {
        if (operand.type() == typeid(int)) {
            return -std::any_cast<int>(operand);
        } else if (operand.type() == typeid(float)) {
            return -std::any_cast<float>(operand);
        } else {
            throw std::runtime_error("Invalid type for unary -");
        }
    }},
    {"=", [](const std::any& operand) -> std::any {
        return operand;
    }}
};


const std::unordered_map<std::string, std::function<std::any(const std::any&, const std::any&)>> Executor::binary_op = {

    {"+", [](const std::any& x, const std::any& y) -> std::any {
        try {
            auto left = std::any_cast<int>(x);
            auto right = std::any_cast<int>(y);
            return left + right;
        } catch (...) {
            try {
                auto left = std::any_cast<float>(x);
                auto right = std::any_cast<float>(y);
                return left + right;
            } catch (...) {
                try {
                    auto left = std::any_cast<std::string>(x);
                    auto right = std::any_cast<std::string>(y);
                    return left + right;
                } catch (...) {
                    std::cout << "No such operand ";
                }
            }
        }
        return std::any();
    }},

    {"-", [](const std::any& x, const std::any& y) -> std::any {
        try {
            auto left = std::any_cast<int>(x);
            auto right = std::any_cast<int>(y);
            return left - right;
        } catch (...) {
            try {
                auto left = std::any_cast<float>(x);
                auto right = std::any_cast<float>(y);
                return left - right;
            } catch (...) {
                try {
                    auto left = std::any_cast<bool>(x);
                    auto right = std::any_cast<bool>(y);
                    return left - right;
                } catch (...) {
                    std::cout << "No such operand ";
                }
            }
        }
        return std::any();
    }},


    {"==", [](const std::any& x, const std::any& y) -> std::any {
        try {
            auto left = std::any_cast<int>(x);
            auto right = std::any_cast<int>(y);
            return left == right;
        } catch (...) {
            try {
               auto left = std::any_cast<float>(x);
                auto right = std::any_cast<float>(y);
                return left == right;
            } catch (...) {
                try {
                    auto left = std::any_cast<std::string>(x);
                    auto right = std::any_cast<std::string>(y);
                    return left == right;
                } catch (...) {
                    try {
                        auto left = std::any_cast<bool>(x);
                        auto right = std::any_cast<bool>(y);
                        return left == right;
                    } catch (...) {
                        std::cout << "No such operand ";
                    }
                }
            }
        }
        return std::any();
    }},
    
  {"=", [](const std::any&, const std::any& y) -> std::any {
        return y;
    }},
};
