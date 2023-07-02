#include "ast_node.h"

namespace front {

namespace parser {

namespace ast {

Node::Node(NodeType type,
     std::optional<lexer::Token>&& token,
     Node* parent,
     std::unique_ptr<Node>&&left,
     std::unique_ptr<Node>&& right):
m_type(type),
m_token(token),
m_parent(parent),
m_left(std::move(left)),
m_right(std::move(right))
{}

NodeType Node::type() const
{
    return m_type;
}

const std::optional<lexer::Token>& Node::getToken() const
{
    return m_token;
}

const Node* Node::getParent() const {
    return m_parent;
}

const Node* Node::getLeft() const {
    return m_left.get();
}

const Node* Node::getRight() const {
    return m_right.get();
}

Node* Node::getParent() {
    return m_parent;
}

Node* Node::getLeft() {
    return m_left.get();
}

Node* Node::getRight() {
    return m_right.get();
}

void Node::setParent(Node* node) {
    m_parent = node;
}

void Node::setLeft(std::unique_ptr<Node>&& node) {
    m_left = std::move(node);
}

void Node::setRight(std::unique_ptr<Node>&& node) {
    m_right = std::move(node);
}

void Node::setToken(lexer::Token&& token) {
    m_token = std::move(token);
}

void Node::setToken(const lexer::Token& token) {
    m_token = token;
}

std::ostream& Node::operator<<(std::ostream& os) const {
    {
        const auto left = getLeft();
        if (left != nullptr) {
            left->operator<<(os);
        }
    }
    {
        const auto token = getToken();
        if (token.has_value()) {
            os << *token;
        }
    }
    {
        const auto right = getRight();
        if (right != nullptr) {
            right->operator<<(os);
        }
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Node& root) {
    return root.operator<<(os);
}

} // namespace ast

} // namespace parser

} // namespace front
