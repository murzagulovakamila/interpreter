#pragma once
#include <string>
#include <vector>
#include <memory>   
#include <iostream>
#include <any>
#include "type.hpp"
#include "value.hpp"
class Visitor;
struct ASTNode{
	virtual void accept(Visitor&) = 0;
	virtual ~ASTNode() = default;
};

using node = std::shared_ptr<ASTNode>;


struct DeclarationNode : ASTNode {
    DeclarationNode() = default;
    virtual ~DeclarationNode() = default;
    std::shared_ptr<Type> type;
    std::string name;
    std::shared_ptr<Value> value;
    void accept(Visitor& visitor);
};
using decl = std::shared_ptr<DeclarationNode>;

struct ProgramNode : public ASTNode {
    std::vector<std::shared_ptr<DeclarationNode>> declarations;
    void accept(Visitor& visitor) override;
};


struct StatementNode : ASTNode {
    StatementNode() = default;
    virtual ~StatementNode() = default;
    void accept(Visitor&) override;
};
using st = std::shared_ptr<StatementNode>;


struct ExpressionNode : ASTNode {
    ExpressionNode() = default;
    virtual ~ExpressionNode() = default;
    std::shared_ptr<Value> value;
    std::shared_ptr<ExpressionNode> expression;
    void accept(Visitor&);
};


using expr = std::shared_ptr<ExpressionNode>;
struct VarDecl : public DeclarationNode {
    std::string type;
    std::vector<std::pair<std::string, expr>> expressions;
	std::shared_ptr<ExpressionNode> initialValue;
    VarDecl(const std::string& t, const std::vector<std::pair<std::string, expr>>& e) 
        : type(t), expressions(e) {}
        void accept(Visitor&);
};


struct ParamDecl : public DeclarationNode {
	std::string type;
	std::string name;
	 ParamDecl(const std::string& first, const std::string& second) 
        : type(first), name(second) {}
        void accept(Visitor&);
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct ConditionalStatement : public StatementNode {
    expr condition;
    st if_statement;
    st else_statement;
    std::shared_ptr<StatementNode> true_branch;
    std::shared_ptr<StatementNode> false_branch;
    ConditionalStatement(const expr& cond, const st& if_stmt, const st& else_st)
        : condition(cond), if_statement(if_stmt), else_statement(else_st) {}
    ConditionalStatement() = default;
    void accept(Visitor&);
};


struct LoopStatement : public StatementNode{
	std::shared_ptr<StatementNode> body;
	virtual ~LoopStatement() = default;
	void accept(Visitor&);
};

struct WhileStatement : public LoopStatement {
    expr condition;
    st while_statement;

    WhileStatement(const expr& cond, const st& while_stmt)
        : condition(cond), while_statement(while_stmt) {}
    WhileStatement() = default;
    void accept(Visitor&);
};

struct DeclarationStatement : public StatementNode {
    std::shared_ptr<VarDecl> declaration;
    DeclarationStatement(const std::shared_ptr<DeclarationNode>& decl)
        : declaration(std::dynamic_pointer_cast<VarDecl>(decl)) {}

    void accept(Visitor&);
};



struct ForStatement : public LoopStatement {
    std::shared_ptr<StatementNode> init;
    std::shared_ptr<ExpressionNode> condition;
    std::shared_ptr<ExpressionNode> increment;
    std::shared_ptr<StatementNode> body;

    ForStatement(std::shared_ptr<StatementNode>& init, std::shared_ptr<ExpressionNode>& condition, std::shared_ptr<ExpressionNode>& increment, std::shared_ptr<StatementNode>& body)
        : init(init), condition(condition), increment(increment), body(body) {}
        void accept(Visitor&) override;
};




struct JumpStatement : public StatementNode{
	JumpStatement() = default;
	virtual ~JumpStatement() = default;
	void accept(Visitor&);
};

struct BreakStatement : public JumpStatement{
	void accept(Visitor&);
};

struct ContinueStatement : public JumpStatement{
	void accept(Visitor&);
};

struct ReturnStatement : public JumpStatement {
    std::shared_ptr<ExpressionNode> return_value;
	std::shared_ptr<ExpressionNode> expr;
    ReturnStatement(std::shared_ptr<ExpressionNode> value) 
        : return_value(value) {}

    void accept(Visitor&) override;
};


struct ExpressionStatement : public StatementNode{
	expr Expr;
	std::shared_ptr<Value> value;
	std::shared_ptr<Type> type;
	ExpressionStatement(const expr& e)
        : Expr(e) {}
        void accept(Visitor&);
};


struct BlockStatement : public StatementNode{
	std::vector<st> statements;
	BlockStatement(std::vector<st>& statements):statements(statements){}
	void accept(Visitor&);
};

struct PrintStatement : public StatementNode {
	void accept(Visitor&);
	std::vector<expr> args;
	PrintStatement(std::vector<expr>& args) : args(args) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





struct FuncDecl : public DeclarationNode {
    std::string type;
    std::string name;
    std::any result;
    std::shared_ptr<BlockStatement> body;
    std::shared_ptr<Type> returnType;
    std::shared_ptr<Value> value;
    std::vector<decl> parameters;
    st block;

    FuncDecl(const std::string& type, const std::string& name,
             const std::vector<decl>& parameters, const st& block)
        : type(type), name(name), parameters(parameters), block(block) {}
        void accept(Visitor&) override;
};



struct BinaryNode : public ExpressionNode {
    std::string name;
    expr rhs, lhs;
	std::string op;
    BinaryNode(const std::string& name, const expr& right, const expr& left)
        : name(name), rhs(right), lhs(left) {}

    BinaryNode() = default;
    void accept(Visitor&);
};


struct UnaryNode : public ExpressionNode {
	std::string op;
    expr branch;
	std::shared_ptr<Value> value;
    UnaryNode(const std::string& op, const expr& branch)
        : op(op), branch(branch) {}
   	virtual ~UnaryNode() = default;
	void accept(Visitor&);
};

struct Prefix : public UnaryNode {
    std::string operation; 
    expr expression;
	std::shared_ptr<Value> value;
    Prefix(const std::string& op, const expr& expr)
        : UnaryNode(op, expr), operation(op), expression(expr) {}

    Prefix() = default;
    virtual ~Prefix() = default;
    void accept(Visitor&);
};

struct Postfix : public UnaryNode {
    std::string operation; 
    expr expression;
	std::shared_ptr<Value> value;
    Postfix(const std::string& op, const expr& expr)
        : UnaryNode(op, expr), operation(op), expression(expr) {}

    Postfix() = default;
    virtual ~Postfix() = default;
     void accept(Visitor&);
};



struct FunctionNode : public ExpressionNode {
    std::string name;
    std::vector<expr> branches;

    FunctionNode(const std::string& func_name, const std::vector<expr>& func_branches)
        : name(func_name), branches(func_branches) {}
         void accept(Visitor&);
};


struct ParenthesizedNode : public ExpressionNode {
    std::vector<std::shared_ptr<DeclarationNode>> declarations;
	std::shared_ptr<ExpressionNode> expr;
    ParenthesizedNode(std::shared_ptr<ExpressionNode> expression) : expr(expression) {}
    void accept(Visitor&) override;

};


struct IdentifierNode : ExpressionNode {
	std::string name;

	IdentifierNode(const std::string& name)
		: name(name) {}
	void accept(Visitor&);
}; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct LiteralNode:ExpressionNode{
	virtual ~LiteralNode() = default;
	void accept(Visitor&);
};

struct LiteralINT : public ExpressionNode {
	int value;
	LiteralINT(int value) : value(value) {}
	void accept(Visitor&);
};

struct LiteralFLOAT : public ExpressionNode {
	float value;
	LiteralFLOAT(float value) : value(value) {}
	void accept(Visitor&);
};

struct LiteralCHAR : public ExpressionNode {
	char value;
	LiteralCHAR(char value) : value(value) {}
	void accept(Visitor&);
};

struct LiteralBOOL : public ExpressionNode {
	bool value;
	LiteralBOOL(bool value) : value(value) {}
	void accept(Visitor&);
};
struct StringLiteralNode : public ExpressionNode{
	std::string value;
	StringLiteralNode(const std::string& value) : value(value) {}
	void accept(Visitor&);
};
struct NumberNode : public ExpressionNode {
	double value;

	NumberNode(double value)
		: value(value) {}
	void accept(Visitor&);
};

