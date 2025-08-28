#ifndef PARSERTOKEN_H
#define PARSERTOKEN_H

#include <string>
#include <vector>
#include "./lexer_token.h" // LexerToken

struct ParserToken {
    std::string value;
    std::string type;
    std::vector<LexerToken> child_tokens;

    inline ParserToken(const std::string& _value, const std::string& _type, const std::vector<LexerToken> _child_tokens)
        : value(_value), type(_type), child_tokens(_child_tokens) {}
    
    inline std::string get_value() { return value; }
    inline std::string get_type() { return type; }
    inline std::string get_children() { return child_tokens; }
    inline std::string get_line() { return child_tokens[0].get_line(); }
}

#endif // PARSERTOKEN_H