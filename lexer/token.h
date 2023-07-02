#pragma once

#include <string>
#include <string_view>
#include <ostream>

namespace front {

namespace lexer {

enum class TokenType {
    Undefined,
    Number,                             // 1,2,23, 34, ..
    Char,                               // 'h', '#', ...
    Bool,                               // true, false
    Str,                                // "Hello", "world", ...

    Plus,                               // +
    Minus,                              // -
    Multiply,                           // *
    Divide,                             // /

    Assign,                             // =
    Less,                               // <
    LessOrEq,                           // <=
    More,                               // >
    MoreOrEq,                           // >=
    Not,                                // !
    NotEq,                              // !=
    Eq,                                 // ==
    And,                                // &
    AndAnd,                             // &&
    Or,                                 // |
    OrOr,                               // ||
    Comma,                              // ,
    Semicolon,                          // ;
    Word,                               // myVariableName, myFunctionName1, myFuncName23, myArray_Name34, ... 

    LeftBrace,                          // {
    RightBrace,                         // }
    LeftBracket,                        // (
    RightBracket,                       // )

    Define,                             // key word: use to define {variable, function, array and etc}
    Variable,                           // key word: define variable
    Function,                           // key word: define function
    While,                              // while
    If,                                 // if
    Else,                               // else
    ElseIf,                             // else if 
};

class Token final {
public:
    explicit Token(TokenType type, std::string&& value);

    TokenType type() const;
    const std::string_view value() const;

private:
    TokenType m_type;
    std::string m_value;
};

std::ostream& operator<<(std::ostream& os, const Token& token);
bool operator==(const Token& rhs, const Token& lhs);

} // namespace lexer

} // namespace front
