#include "base_expr_parser.h"
#include "lexer/token.h"

#include <cassert>

namespace {

bool IsPrimitive(const front::lexer::TokenType type) {
    switch (type) {
        using TokenType = front::lexer::TokenType;
        case TokenType::Number:
        case TokenType::Str:
        case TokenType::Char:
        case TokenType::Bool:
            return true;
        default:
            return false;
    }

    return false;
}

} // namespace

namespace front {

namespace parser {

std::unique_ptr<Parser::AstNode> ParserBaseExpr(const Parser::TokensType tokens) {
    // stmt -> primitive | plus_minus;
    // plus_minus -> {+,-} stmt | mult_div;
    // mult_div -> {*, /} stmt | primitive;
    // pritive -> {NUMER, STRING, ...} token;
    using namespace ast;
    using namespace lexer;

    auto rootNode = std::make_unique<Node>(NodeType::BaseStatement);
    auto* currRootNode = rootNode.get();
    Parser::PosType pos = 0;
    const auto size = tokens.size();
    while (pos < size) {
        const auto token = tokens[pos];

        switch (currRootNode->type()) {
            case NodeType::BaseStatement: {
                auto newNode = std::make_unique<Node>(NodeType::Plus_Minus);
                const auto plusMinusNode = newNode.get();

                newNode->setParent(currRootNode);
                if (currRootNode->getLeft() == nullptr) {
                    currRootNode->setLeft(std::move(newNode));
                } else {
                    currRootNode->setRight(std::move(newNode));
                }

                currRootNode = plusMinusNode;
                break;
            }
            case NodeType::Plus_Minus: {
                if (const auto tokeType = token.type();
                        tokeType == TokenType::Plus || tokeType == TokenType::Minus) {
                    currRootNode->setToken(token);
                    ++pos;

                    auto newNode = std::make_unique<Node>(NodeType::BaseStatement);
                    const auto stmtNode = newNode.get();

                    newNode->setParent(currRootNode);
                    if (currRootNode->getLeft() == nullptr) {
                        currRootNode->setLeft(std::move(newNode));
                    } else {
                        currRootNode->setRight(std::move(newNode));
                    }


                    currRootNode = stmtNode;
                    break;
                } else {
                    auto newNode = std::make_unique<Node>(NodeType::Multiply_Divide);
                    const auto multDivNode = newNode.get();

                    newNode->setParent(currRootNode);
                    if (currRootNode->getLeft() == nullptr) {
                        currRootNode->setLeft(std::move(newNode));
                    } else {
                        currRootNode->setRight(std::move(newNode));
                    }

                    currRootNode = multDivNode;
                    break;
                }
            }
            case NodeType::Multiply_Divide: {
                if (const auto tokeType = token.type();
                        tokeType == TokenType::Multiply || tokeType == TokenType::Divide) {
                    currRootNode->setToken(token);
                    ++pos;

                    auto newNode = std::make_unique<Node>(NodeType::BaseStatement);
                    const auto stmtNode = newNode.get();

                    newNode->setParent(currRootNode);
                    if (currRootNode->getLeft() == nullptr) {
                        currRootNode->setLeft(std::move(newNode));
                    } else {
                        currRootNode->setRight(std::move(newNode));
                    }

                    currRootNode = stmtNode;
                    continue;
                } else if (tokeType == TokenType::Plus || tokeType == TokenType::Minus) {
                    currRootNode = currRootNode->getParent();
                } else {
                    auto newNode = std::make_unique<Node>(NodeType::Primitive, token);
                    const auto prmtvNode = newNode.get();

                    newNode->setParent(currRootNode);
                    if (currRootNode->getLeft() == nullptr) {
                        currRootNode->setLeft(std::move(newNode));
                    } else {
                        currRootNode->setRight(std::move(newNode));
                    }

                    ++pos;
                }
                break;
            }
            default: {
                assert(false);
            }
        }
    }

    return rootNode;
}

} // namespace parser

} // namespace front
