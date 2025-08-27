#ifndef LEXERTOKEN_H
#define LEXERTOKEN_H 

#include <iostream>
#include <string>

#pragma once

struct LexerToken {
    std::string value;
    std::string type;
    size_t line;

    inline LexerToken(const std::string& _value, const std::string& _type, const size_t& _line)
        : value(_value), type(_type), line(_line) {}

    inline std::string get_value() { return value; }
    inline std::string get_type() { return type; }
    inline size_t get_line() { return line; }
};

#endif // LEXERTOKEN_H