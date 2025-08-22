#ifndef LEXERTOKEN_H
#define LEXERTOKEN_H 

#include <string>


struct LexerToken {
    std::string value;
    std::string type;

    LexerToken(const std::string& _value, const std::string& _type)
        : value(_value), type(_type) {}
    
    std::string get_value() {
        return value;
    }

    std::string get_type() {
        return type;
    }
};

#endif // LEXERTOKEN_H