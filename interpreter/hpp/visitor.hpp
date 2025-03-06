#include "node.hpp"
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <any>
#include "symbol.hpp"

using func_dict = std::unordered_map<std::string, node>;
using var_set = std::unordered_set<std::string>;

class Visitor {
public:
    virtual void visit(ParamDecl&) = 0;
    virtual void visit(VarDecl&) = 0; 
    virtual void visit(StatementNode&) = 0; 
    virtual void visit(DeclarationNode&) = 0; 
    virtual void visit(BinaryNode&) = 0;
    virtual void visit(UnaryNode&) = 0;
    virtual void visit(FunctionNode&) = 0;
    virtual void visit(IdentifierNode&) = 0;
    virtual void visit(NumberNode&) = 0;
    virtual void visit(ParenthesizedNode&) = 0;
    virtual void visit(BlockStatement&) = 0;
    virtual void visit(ExpressionNode&) = 0;
    virtual void visit(ReturnStatement&) = 0;
    virtual void visit(ContinueStatement&) = 0;
    virtual void visit(BreakStatement&) = 0;
    virtual void visit(JumpStatement&) = 0;
    virtual void visit(ForStatement&) = 0;
    virtual void visit(DeclarationStatement&) = 0;
    virtual void visit(WhileStatement&) = 0;
    virtual void visit(LoopStatement&) = 0;
    virtual void visit(ConditionalStatement&) = 0;
    virtual void visit(ExpressionStatement&) = 0;
    virtual void visit(PrintStatement&) = 0;
    virtual void visit(FuncDecl&) = 0;
    virtual void visit(Prefix&) = 0;
    virtual void visit(Postfix&) = 0;
    virtual void visit(LiteralNode&) = 0;
    virtual void visit(LiteralINT&) = 0; 
    virtual void visit(LiteralFLOAT&) = 0;
    virtual void visit(LiteralCHAR&) = 0;
    virtual void visit(LiteralBOOL&) = 0;
    virtual void visit(StringLiteralNode&) = 0;
    virtual ~Visitor() = default;
};

class Printer : public Visitor {
public:
    void visit(ParamDecl&) override;
    void visit(VarDecl&) override;  
    void visit(StatementNode&) override; 
    void visit(DeclarationNode&) override; 
    void visit(BinaryNode&) override;
    void visit(UnaryNode&) override;
    void visit(FunctionNode&) override;
    void visit(IdentifierNode&) override;
    void visit(NumberNode&) override;
    void visit(ParenthesizedNode&) override;
    void visit(ExpressionStatement&) override;
    void visit(BlockStatement&) override;
    void visit(ExpressionNode&) override;
    void visit(ContinueStatement&) override;
    void visit(BreakStatement&) override;
    void visit(JumpStatement&) override;
    void visit(ForStatement&) override;
    void visit(DeclarationStatement&) override;
    void visit(WhileStatement&) override;
    void visit(LoopStatement&) override;
    void visit(ConditionalStatement&) override;
    void visit(ReturnStatement&) override;
    void visit(PrintStatement&) override;
    void visit(FuncDecl&) override;
    void visit(Prefix&) override;
    void visit(Postfix&) override;
    void visit(LiteralNode&) override;
    void visit(LiteralINT&) override; 
    void visit(LiteralFLOAT&) override;
    void visit(LiteralCHAR&) override;
    void visit(LiteralBOOL&) override;
    void visit(StringLiteralNode&) override;
    void print(const node&);
};

class Analyzer : public Visitor {
public:
    std::shared_ptr<Scope> analyze(const node&);
    void visit(ParamDecl&) override;
    void visit(VarDecl&) override;  
    void visit(StatementNode&); 
    void visit(DeclarationNode&); 
    void visit(BinaryNode&) override;
    void visit(UnaryNode&) override;
    void visit(FunctionNode&) override;
    void visit(IdentifierNode&) override;
    void visit(NumberNode&) override; 
    void visit(ParenthesizedNode&) override;
    void visit(ExpressionStatement&) override;
    void visit(BlockStatement&) override;
    void visit(ExpressionNode&) override;
    void visit(ContinueStatement&) override;
    void visit(BreakStatement&) override;
    void visit(JumpStatement&) override;
    void visit(ForStatement&) override;
    void visit(DeclarationStatement&) override;
    void visit(WhileStatement&) override;
    void visit(LoopStatement&);
    void visit(ConditionalStatement&) override;
    void visit(ReturnStatement&) override;
    void visit(PrintStatement&) override;
    void visit(FuncDecl&) override;
    void visit(Prefix&) override;
    void visit(Postfix&) override;
    void visit(LiteralNode&) override;
    void visit(LiteralINT&) override;
    void visit(LiteralFLOAT&) override;
    void visit(LiteralCHAR&) override;
    void visit(LiteralBOOL&) override;
    void visit(StringLiteralNode&) override;

private:

    static const std::unordered_map<std::string, std::function<std::any(const std::any&)>> unary_op;
    static const std::unordered_map<std::string, std::function<std::any(const std::any&, const std::any&)>> binary_op;
    static std::string any_to_string(const std::any&);

    ScopeManager scopeManager;
    SymbolTable symbolTable;
    bool returnFlag; 
    bool insideLoop = false; 
    bool cont = false;
    bool brk = false;

    var_set vars;
    func_dict custom_funcs;
    std::any func_res;
    std::any result;
};

class Executor : public Visitor {
public:
    Executor() = default;
    void visit(ParamDecl&) override;
    void visit(VarDecl&) override;  
    void visit(StatementNode&) override; 
    void visit(DeclarationNode&) override; 
    void visit(BinaryNode&) override;
    void visit(UnaryNode&) override;
    void visit(FunctionNode&) override;
    void visit(IdentifierNode&) override;
    void visit(ParenthesizedNode&) override;
    void visit(ExpressionStatement&) override;
    void visit(BlockStatement&) override;
    void visit(ExpressionNode&) override;
    void visit(ContinueStatement&) override;
    void visit(BreakStatement&) override;
    void visit(ForStatement&) override;
    void visit(DeclarationStatement&) override;
    void visit(WhileStatement&) override;
    void visit(ConditionalStatement&) override;
    void visit(ReturnStatement&) override;
    void visit(FuncDecl&) override;
    void visit(JumpStatement&) override;
    void visit(LoopStatement&) override;
    void visit(PrintStatement&) override;
    void visit(NumberNode&) override; 
    void visit(Prefix&) override;
    void visit(Postfix&) override;
    void visit(LiteralNode&) override;
    void visit(LiteralINT&) override;
    void visit(LiteralFLOAT&) override;
    void visit(LiteralCHAR&) override;
    void visit(LiteralBOOL&) override;
    void visit(StringLiteralNode&) override;
    void execute(decl& root);
    std::shared_ptr<Variable> get_variable(std::string&);
    bool lookup(std::string&);
    bool check(std::any);
private:

    static const std::unordered_map<std::string, std::function<std::any(const std::any&)>> unary_op;
    static const std::unordered_map<std::string, std::function<std::any(const std::any&, const std::any&)>> binary_op;
    static std::string any_to_string(const std::any&);

    ScopeManager scopeManager;
    std::any result;
    std::shared_ptr<Variable> current_id;
    std::shared_ptr<ExpressionNode> current_value;
    bool returnFlag = false;
    int break_flag = 0, continue_flag = 0;
};
