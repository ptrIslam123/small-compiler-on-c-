#pragma once

#include "token.h"

#include <string>
#include <string_view>
#include <list>
#include <optional>

namespace front {
    
namespace lexer {
    
class Lexer final {
public:
    using ContainerType = std::list<Token>;
    using PosType = std::string::size_type;

    explicit Lexer();

    ContainerType tokenize(const std::string& text);

private:
    std::optional<Token> processAsString();
    std::optional<Token> processAsChar();
    
    std::optional<Token> processAsKeyWord();
    std::optional<Token> processAsOperator();

    Token processNumber();
    Token processWord();
    void markedAboutLexerError(std::string&& errorMsg);

    char peek(PosType offset = 0) const;

    std::string_view m_text;
    PosType m_pos;
    ContainerType m_tokens;
};

} // namespace lexer

} // namespace front
