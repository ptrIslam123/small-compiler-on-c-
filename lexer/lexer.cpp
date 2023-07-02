#include "lexer.h"

#include "utils/lazy.h"

#include <unordered_map>
#include <array>
#include <sstream>

#include <cctype>
#include <cassert>

namespace front {
    
namespace lexer {
    
Lexer::Lexer():
m_text(),
m_pos(0),
m_tokens()
{}

Lexer::ContainerType Lexer::tokenize(const std::string& text) {
    m_text = text;
    m_pos = 0;
    m_tokens.clear();

    while (m_pos < m_text.size()) {
        const auto symbol = peek();

        if (std::isspace(symbol) || symbol == '\n' || symbol == '\r') {
            m_pos += 1;
            continue;
        }

        if (symbol == '\"') {
            auto result = processAsString();
            if (!result) {
                m_tokens.push_back(std::move(*result));
            } else {
                // handle error
            }

            continue;
        }

        if (symbol == '\'') {
            auto result = processAsChar();
            if (!result) {
                m_tokens.push_back(std::move(*result));
            } else {
                // handle error
            }

            continue;
        }

        {
            const auto result = processAsKeyWord();
            if (result) {
                m_tokens.push_back(std::move(*result));
                continue;
            } else {
                // just pass
            }
        }

        {
            const auto result = processAsOperator();
            if (result) {
                m_tokens.push_back(std::move(*result));
                continue;
            } else {
                // just pass
            }
        }

        if (std::isdigit(symbol)) {
            m_tokens.push_back(processNumber());
            continue;
        }

        if (std::isalpha(symbol)) {
            m_tokens.push_back(processWord());
            continue;
        }

        // handle unknown token(symbol)!!!
    }

    return m_tokens;
}

std::optional<Token> Lexer::processAsString() {
    ++m_pos;
    std::string buffer;
    const auto textSize = m_text.size();
    while (m_pos < textSize) {
        const auto symbol = peek();
        if (symbol != '\"') {
            buffer.push_back(symbol);
            ++m_pos;
            continue;
        } else if (symbol == '\"') {
            ++m_pos;
            return Token(TokenType::Char, std::move(buffer));
        } else {
            break;
        }
    }

    std::stringstream ss;
    ss << "Invalid string type. The string have to end up symbol=(\")";
    markedAboutLexerError(ss.str());
    ++m_pos;
    return std::nullopt;
}

std::optional<Token> Lexer::processAsChar() {
    ++m_pos;
    std::string buffer;
    const auto textSize = m_text.size();
    while (m_pos < textSize || buffer.size() > 1) {
        const auto symbol = peek();
        if (symbol != '\"') {
            buffer.push_back(symbol);
            ++m_pos;
            continue;
        } else if (symbol == '\"') {
            ++m_pos;
            return Token(TokenType::Char, std::move(buffer));
        } else {
            break;
        }
    }

    std::stringstream ss;
    ss << "Unexpected symbol=(" << peek() << "), expected symbol=(\')";
    markedAboutLexerError(ss.str());
    ++m_pos;
    return std::nullopt;
}

std::optional<Token> Lexer::processAsKeyWord() {
    static const std::unordered_map<std::string_view, TokenType> keyWords = {
        {"def", TokenType::Define},
        {"var", TokenType::Variable},
        {"func", TokenType::Function},
        {"while", TokenType::While},
        {"if", TokenType::If},
        {"else", TokenType::Else},
        {"elseif", TokenType::ElseIf},
        {"true", TokenType::Bool},
        {"false", TokenType::Bool}
    };

    const auto firstSymbol = peek();
    const auto textSize = m_text.size();
    std::string buffer;
    auto pos = 0;

    for (const auto& keyWord : keyWords) {
        const auto& curKeyWord = keyWord.first;
        if (firstSymbol == curKeyWord[0]) {
            buffer.push_back(firstSymbol);
            pos += 1;

            while (m_pos + pos <= textSize) {
                if (buffer.size() >= curKeyWord.size()) {
                    const auto it = keyWords.find(buffer);
                    assert(it != keyWords.cend());
                    m_pos += pos;
                    return Token(it->second, std::move(buffer));
                }

                const auto symbol = peek(pos);
                if (symbol == curKeyWord[pos]) {
                    buffer.push_back(symbol);
                    pos += 1;
                    continue;
                }

                pos = 0;
                buffer.clear();
                break;
            }
        }
    }

    return std::nullopt;
}

std::optional<Token> Lexer::processAsOperator() {
    static const std::unordered_map<char, TokenType> optTable = {
        {'+', TokenType::Plus},
        {'-', TokenType::Minus},
        {'*', TokenType::Multiply},
        {'/', TokenType::Divide},
        {'(', TokenType::LeftBracket},
        {')', TokenType::RightBracket},
        {'{', TokenType::LeftBrace},
        {'}', TokenType::RightBrace},
        {'=', TokenType::Assign},
        {'<', TokenType::Less},
        {'>', TokenType::More},
        {'&', TokenType::And},
        {'|', TokenType::More},
        {'!', TokenType::NotEq},
        {',', TokenType::Comma},
        {';', TokenType::Semicolon}
    };

    const auto textSize = m_text.size();
    std::array<utils::LazyConstructedObject<Token>, 2> buffer;
    auto bufferSize = 0;
    auto i = 0;

    while (m_pos < textSize && i < 2) {
        const auto symbol = peek();
        const auto it = optTable.find(symbol);
        if (it != optTable.cend()) {
            std::string value;
            value.push_back(symbol);
            buffer[bufferSize] = Token(it->second, std::move(value));

            ++m_pos; ++i; ++bufferSize;
            continue;
        } else {
            break;
        }
    }

    switch (bufferSize) {
        case 1: {
            return *buffer[0];
        }
        case 2: {
            const auto firstType = buffer[0]->type();
            const auto secondType = buffer[1]->type();
            if (firstType == TokenType::Less && secondType == TokenType::Assign) {
                return Token(TokenType::LessOrEq, std::string("<="));
            } else if (firstType == TokenType::More && secondType == TokenType::Assign) {
                return Token(TokenType::MoreOrEq, std::string(">="));
            } else if (firstType == TokenType::Assign && secondType == TokenType::Assign) {
                return Token(TokenType::Eq, std::string("=="));
            } else if (firstType == TokenType::Not && secondType == TokenType::Assign) {
                return Token(TokenType::NotEq, std::string("!="));
            } else if (firstType == TokenType::And && secondType == TokenType::And) {
                return Token(TokenType::AndAnd, std::string("&&"));
            } else if (firstType == TokenType::Or && secondType == TokenType::Or) {
                return Token(TokenType::OrOr, std::string("||"));
            } else {
                std::stringstream ss;
                ss  << "LEXER: Unknown operators=("
                    << buffer[0]->value() << ", " << buffer[1]->value()
                    << ")";
                markedAboutLexerError(ss.str());
            }
        }
        default: {
            return std::nullopt;
        }
    }
}

Token Lexer::processNumber() {
    std::string buffer;
    buffer.push_back(peek());
    ++m_pos;

    const auto textSize = m_text.size();
    while (m_pos < textSize) {
        const auto symbol = peek();
        if (std::isdigit(symbol)) {
            buffer.push_back(symbol);
            ++m_pos;
            continue;
        }
        break;
    }

    return Token(TokenType::Number, std::move(buffer));
}


Token Lexer::processWord() {
    std::string buffer;
    buffer.push_back(peek());
    ++m_pos;

    const auto textSize = m_text.size();
    while (m_pos < textSize) {
        const auto symbol = peek();
        if (std::isalpha(symbol) || std::isdigit(symbol) || symbol == '_') {
            buffer.push_back(symbol);
            ++m_pos;
            continue;
        }
        break;
    }

    return Token(TokenType::Word, std::move(buffer));
}

void Lexer::markedAboutLexerError(std::string&& errorMsg) {
    Token undefinedToken(TokenType::Undefined, std::move(errorMsg));
    if (!m_tokens.empty() && m_tokens.back() != undefinedToken) {
        m_tokens.push_back(std::move(undefinedToken));
    }
}

char Lexer::peek(PosType offset) const {
    assert(m_pos + offset <= m_text.size());
    return m_text[m_pos + offset];
}

} // namespace lexer


} // namespace front
