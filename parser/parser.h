#pragma once

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/ast_node.h"

#include <memory>
#include <span>

namespace front {

namespace parser {

class Parser {
public:
    using TokenType = lexer::Token;
    using TokensType = std::span<TokenType>;
    using PosType = lexer::Lexer::PosType;
    using AstNode = ast::Node;

    explicit Parser();
    std::unique_ptr<AstNode> parse(TokensType tokens);

private:
    TokensType m_tokens;
    PosType m_pos;
};

} // namespace parser

} // namespace front
