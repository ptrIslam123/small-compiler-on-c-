#pragma once

#include "lexer/token.h"

#include <memory>
#include <optional>
#include <ostream>

namespace front {

namespace parser {

namespace ast {

enum class NodeType {
    BaseStatement,
    Plus_Minus,
    Multiply_Divide,
    Primitive,
};

class Node final {
public:
    Node(NodeType type,
         std::optional<lexer::Token>&& token = std::nullopt,
         Node* parent = nullptr,
         std::unique_ptr<Node>&& left = std::unique_ptr<Node>(),
         std::unique_ptr<Node>&& right = std::unique_ptr<Node>());

    NodeType type() const;
    const std::optional<lexer::Token>& getToken() const;

    const Node* getParent() const;
    const Node* getLeft() const;
    const Node* getRight() const;

    Node* getParent();
    Node* getLeft();
    Node* getRight();

    void setToken(lexer::Token&& token);
    void setToken(const lexer::Token& token);
    void setParent(Node* node);
    void setLeft(std::unique_ptr<Node>&& node);
    void setRight(std::unique_ptr<Node>&& node);

    std::ostream& operator<<(std::ostream& os) const;

private:
    NodeType m_type;
    std::optional<lexer::Token> m_token;
    Node* m_parent;
    std::unique_ptr<Node> m_left;
    std::unique_ptr<Node> m_right;
};

std::ostream& operator<<(std::ostream& os, const Node& root);

} // namespace ast

} // namespace parser

} // namespace front
