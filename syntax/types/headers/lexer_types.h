#ifndef LEXERTYPES_H
#define LEXERTYPES_H

#include <iostream>
#include <string>

struct LexerTypes {
    std::string ENDL() {
        return "ENDL";
    }

    std::string ENDF() {
        return "EOF";
    }

    std::string CONST() {
        return "CONST";
    }

    std::string LET() {
        return "let";
    }

    std::string STRING() {
        return "STRING";
    }

    std::string INTEGER() {
        return "INTEGER";
    }

    std::string FLOAT() {
        return "FLOAT";
    }

    std::string ARRAY() {
        return "ARRAY";
    }

    std::string TUPLE() {
        return "TUPLE";
    }

    std::string OBJECT() {
        return "OBJECT";
    }

    std::string FUNC() {
        return "FUNC";
    }

    std::string ANON_FUNC() {
        return "ANON_FUNC";
    }

    std::string KEYWORD() {
        return "KEYWORD";
    }

    std::string DECL() {
        return "DECL";
    }

    std::string SELECTOR() {
        return "SELECTOR";
    }

    std::string OPERATOR() {
        return "OPERATOR";
    }

    std::string METHOD() {
        return "METHOD";
    }

    std::string ATTR() {
        return "ATTR";
    }

    std::string DECL_METHOD() {
        return "DECL_METHOD";
    }

    std::string CLASS() {
        return "CLASS";
    }

    std::string PROTO() {
        return "PROTOYPE";
    }
};

#endif // LEXERTYPES_H