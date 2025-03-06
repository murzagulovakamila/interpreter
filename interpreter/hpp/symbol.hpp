#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <stack>

#include "type.hpp"
#include "value.hpp"

struct CompoundStatement;

struct Symbol {
    virtual ~Symbol() noexcept = default;
};


struct Variable : public Symbol {
    std::shared_ptr<Type> type;
    std::any value;
};

struct Function : public Symbol {
    std::shared_ptr<Type> returnType;
    std::vector<std::pair<std::string, std::shared_ptr<Variable>>> arguments;
    std::shared_ptr<BlockStatement> body;
};

using SymbolTable = std::unordered_map<std::string, std::shared_ptr<Symbol>>;

struct Scope {
    SymbolTable table;
    std::shared_ptr<Scope> parent;

    Scope(const std::shared_ptr<Scope> parent = nullptr) : parent(parent) {}

    void add(const std::string& name, const std::shared_ptr<Symbol>& symbol) {
        if(table.contains(name)) {
            throw std::runtime_error("Redeclaration of symbol " + name + ".");
        }
        table[name] = symbol;
    }

    std::shared_ptr<Variable> get_variable(const std::string& name) {
    	if(!table.contains(name)) {
    		return parent->get_variable(name);	
    	}
    	return std::dynamic_pointer_cast<Variable>(table[name]);
    }

    bool lookup(const std::string& name) {
        if (!table.contains(name)) {
            if (parent == nullptr) {
                return false;
            }
            return parent->lookup(name);
        }
        return true;
    }
};

struct ScopeManager {
    std::stack<std::shared_ptr<Scope>> scopes;

    ScopeManager() {
        scopes.push(std::make_shared<Scope>());
    }

    void enterScope() {
        scopes.push(std::make_shared<Scope>(scopes.top()));
    }

    void exitScope(){
        scopes.pop();
    }
};

