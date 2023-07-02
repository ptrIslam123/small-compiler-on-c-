#include "token.h"

namespace front {
    
namespace lexer{

Token::Token(const TokenType type, std::string&& value):
m_type(type),
m_value(std::move(value))
{}

TokenType Token::type() const {
    return m_type;
}

const std::string_view Token::value() const {
    return m_value;
}

bool operator==(const Token& rhs, const Token& lhs)
{
    return rhs.type() == lhs.type() && rhs.value() == lhs.value();
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "type=\'";
    switch (token.type()) {
        case TokenType::Undefined: {
            os << "Undefined";
            break;
        }
        case TokenType::Number: {
            os << "Number";
            break;
        }
        case TokenType::Char: {
            os << "Char";
            break;
        }
        case TokenType::Bool: {
            os << "Bool";
            break;
        }
        case TokenType::Str: {
            os << "Str";
            break;
        }
        case TokenType::Plus: {
            os << "Plus";
            break;
        }
        case TokenType::Minus: {
            os << "Minus";
            break;
        }
        case TokenType::Multiply: {
            os << "Multiply";
            break;
        }
        case TokenType::Divide: {
            os << "Divide";
            break;
        }
        case TokenType::Assign: {
            os << "Assign";
            break;
        }
        case TokenType::Less: {
            os << "Less";
            break;
        }
        case TokenType::LessOrEq: {
            os << "LessOrEq";
            break;
        }
        case TokenType::More: {
            os << "More";
            break;
        }
        case TokenType::MoreOrEq: {
            os << "MoreOrEq";
            break;
        }
        case TokenType::NotEq: {
            os << "NotEq";
            break;
        }
        case TokenType::Eq: {
            os << "Eq";
            break;
        }
        case TokenType::And: {
            os << "And";
            break;
        }
        case TokenType::AndAnd: {
            os << "AndAnd";
            break;
        }
        case TokenType::Or: {
            os << "Or";
            break;
        }
        case TokenType::OrOr: {
            os << "OrOr";
            break;
        }
        case TokenType::Comma: {
            os << "Comma";
            break;
        }
        case TokenType::Semicolon: {
            os << "Semicolon";
            break;
        }
        case TokenType::Word: {
            os << "Word";
            break;
        } 
        case TokenType::LeftBrace: {
            os << "LeftBrace";
            break;
        }
        case TokenType::RightBrace: {
            os << "RightBrace";
            break;
        }
        case TokenType::LeftBracket: {
            os << "LeftBracket";
            break;
        }
        case TokenType::RightBracket: {
            os << "RightBracket";
            break;
        }
        case TokenType::Define: {
            os << "Define";
            break;
        }
        case TokenType::Variable: {
            os << "Variable";
            break;
        }
        case TokenType::Function: {
            os << "Function";
            break;
        }
        case TokenType::While: {
            os << "While";
            break;
        }
        case TokenType::If: {
            os << "If";
            break;
        }
        case TokenType::Else: {
            os << "Else";
            break;
        }
        case TokenType::ElseIf: {
            os << "ElseIf";
            break;
        }   
        default: {
            os << "Undefined token";
        }
    }
    os << "\':value=\'" << token.value() << "\'";
    return os;
}

} // namespace lexer


} // namespace front
