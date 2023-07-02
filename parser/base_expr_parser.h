#pragma once

#include "parser/parser.h"

#include <memory>
#include <span>

namespace front {

namespace parser {

std::unique_ptr<Parser::AstNode> ParserBaseExpr(Parser::TokensType tokens);

} // namespace parser

} // namespace front
