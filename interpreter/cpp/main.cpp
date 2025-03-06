#include "lexer.hpp" 
#include "parser.hpp" 
#include "visitor.hpp"
#include "token.hpp"


void print() {
	std::string text;
        std::cout << text << std::endl << "\n\n";
    }

void print(std::vector<Token>& tokens) {
    for (auto it = tokens.begin(); it != tokens.end(); it++) {
	    it->print();
    }
}
int main(){

    std::ifstream file("input_code.cpp");
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string input_code; 
    std::string line;
	while (std::getline(file, line)) {
		input_code += line + "\n";
	}

    file.close();
	
    Lexer lexer(input_code);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    std::vector<decl> declarations = parser.parse();

    std::cout << "Parsing completed successfully" << std::endl;

    Analyzer analyzer;
    for (const auto& decl : declarations) {
        analyzer.analyze(decl);
    }
	std::cout << "Analyzer completed successfully" << std::endl;
	
	
    Printer printer;
    for (const auto& decl : declarations) {
        decl->accept(printer);
    }
    std::cout << "Printing completed successfully" << std::endl;
   
    Executor executor;
    executor.execute(declarations[0]);

    std::cout << "Executor completed successfully" << std::endl;
    
    return 0;
}
