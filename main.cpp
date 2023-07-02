#include <iostream>

#include "lexer/token.h"
#include "lexer/lexer.h"

using namespace front;
using namespace lexer;

int main() {
    const std::string code = "def var myVariabl1 = 10  + 23/  12 && 23 >= 23";

    Lexer lexer;
    const auto tokens = lexer.tokenize(code);

    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }
    return 0;
}
