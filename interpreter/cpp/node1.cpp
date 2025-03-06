#include "visitor.hpp"

void DeclarationNode::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void StatementNode::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void ExpressionNode::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void VarDecl::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void ParamDecl::accept(Visitor& visitor){
	visitor.visit(*this);
};

///////////////////////////////////////////////////////////////

void ConditionalStatement::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void LoopStatement::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void WhileStatement::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void DeclarationStatement::accept(Visitor& visitor) {
	visitor.visit(*this);
};


void ForStatement::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void JumpStatement::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void BreakStatement::accept(Visitor& visitor){
	visitor.visit(*this);
};

void ContinueStatement::accept(Visitor& visitor){
	visitor.visit(*this);
};

void ReturnStatement::accept(Visitor& visitor){
	visitor.visit(*this);
};

void ExpressionStatement::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void BlockStatement::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void PrintStatement::accept(Visitor& visitor) {
    visitor.visit(*this);
}

////////////////////////////////////////

void FuncDecl::accept(Visitor& visitor){
	visitor.visit(*this);
};

void BinaryNode::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void UnaryNode::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void Prefix::accept(Visitor& visitor){
	visitor.visit(*this);
};

void Postfix::accept(Visitor& visitor){
	visitor.visit(*this);
};

void FunctionNode::accept(Visitor& visitor) {
	visitor.visit(*this);
};

void ParenthesizedNode::accept(Visitor& visitor){
	visitor.visit(*this);
};

void IdentifierNode::accept(Visitor& visitor) {
	visitor.visit(*this);
}; 

/////////////////////////////////////////////////////////////////////////////

void LiteralNode::accept(Visitor& visitor){
	visitor.visit(*this);
};

void LiteralINT :: accept(Visitor& visitor) {
	visitor.visit(*this);
};

void LiteralFLOAT :: accept(Visitor& visitor) {
	visitor.visit(*this);
};

void LiteralCHAR :: accept(Visitor& visitor) {
	visitor.visit(*this);
};

void LiteralBOOL :: accept(Visitor& visitor) {
	visitor.visit(*this);
};
void StringLiteralNode :: accept(Visitor& visitor){
	visitor.visit(*this);
};
void NumberNode :: accept(Visitor& visitor) {
	visitor.visit(*this);
};


