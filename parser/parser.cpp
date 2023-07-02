#include "parser.h"

#include "base_expr_parser.h"

namespace front {

namespace parser {

Parser::Parser():
m_tokens(),
m_pos(0)
{}

std::unique_ptr<Parser::AstNode> Parser::parse(TokensType tokens)
{
    m_tokens = tokens;
    m_pos = 0;
    return ParserBaseExpr(m_tokens);
}

} // namespace parser

} // namespace front
