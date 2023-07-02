#include <iostream>

#include "lexer/token.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

using namespace front;
using namespace lexer;
using namespace parser;

int main() {
    const std::string code = "2 + 3 *   4";

    Lexer lexer;
    auto tokens = lexer.tokenize(code);

    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }
    std::cout << "============================ PARSER ============================ " << std::endl << std::endl;
    Parser parser;
    auto rootNode = parser.parse(tokens);

    std::cout << *rootNode << std::endl;
    return 0;
}
