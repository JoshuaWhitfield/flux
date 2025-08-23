#ifndef LEXERTYPES_H
#define LEXERTYPES_H

#include <iostream>
#include <string>

struct LexerTypes {
    std::string COMMA() {
        return "COMMA";
    }

    std::string REF() {
        return "REF";
    }

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

    std::string ARRAY_START() {
        return "ARRAY_START";
    }

    std::string ARRAY_END() {
        return "ARRAY_END";
    }

    std::string OBJECT_START() {
        return "OBJECT_START";
    }

    std::string OBJECT_END() {
        return "OBJECT_END";
    }

    std::string TUPLE_START() {
        return "TUPLE_START";
    }

    std::string TUPLE_END() {
        return "TUPLE_END";
    }

    std::string FUNC() {
        return "FUNC";
    }

    std::string ANON_FUNC() {
        return "ANON_FUNC";
    }

    std::string KEYWORD() { // return, case, concede, default
        return "KEYWORD";
    }

    std::string CORE_FUNC() { // print, input, len, type, panic
        return "CORE_FUNC";
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

    std::string CHILD() {
        return "CHILD";
    }

    // std::string METHOD() {
    //     return "METHOD";
    // }

    // std::string ATTR() {
    //     return "ATTR";
    // }

    std::string DECL_METHOD() {
        return "DECL_METHOD";
    }

    std::string CLASS() {
        return "CLASS";
    }

    std::string PROTO() {
        return "PROTOYPE";
    }

    std::string UNEXP() {
        return "UNEXPECTED";
    }
};

#endif // LEXERTYPES_H