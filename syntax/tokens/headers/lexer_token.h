#ifndef LEXERTOKEN_H
#define LEXERTOKEN_H 

#include <iostream>
#include <string>

#pragma once

struct LexerToken {
    std::string value;
    std::string type;

    inline LexerToken(const std::string& _value, const std::string& _type)
        : value(_value), type(_type) {}

    inline std::string get_value() { return value; }
    inline std::string get_type() { return type; }
};

#endif // LEXERTOKEN_H