#ifndef LEXERTYPES_H
#define LEXERTYPES_H

#pragma once
#include <string>

struct LexerTypes {
    inline static std::string NORMALIZER() { return "NORMALIZER"; } // Added for your lexer
    inline static std::string NAMESPACE() { return "NAMESPACE"; }
    inline static std::string SELECTOR() { return "SELECTOR"; }
    inline static std::string KEYWORD() { return "KEYWORD"; }
    inline static std::string ASSIGN() { return "ASSIGNMENT"; }
    inline static std::string COMMENT() { return "COMMENT"; }
    inline static std::string COMMENT_START() { return "COMMENT_START"; }
    inline static std::string COMMENT_END() { return "COMMENT_END"; }
    inline static std::string COMMA() { return "COMMA"; }
    inline static std::string ENDL() { return "ENDL"; }
    inline static std::string ENDF() { return "EOF"; }
    inline static std::string REF() { return "REF"; }
    inline static std::string LET() { return "LET"; }
    inline static std::string CONST() { return "CONST"; }
    inline static std::string INTEGER() { return "INTEGER"; }
    inline static std::string STRING() { return "STRING"; }
    inline static std::string FLOAT() { return "FLOAT"; }
    inline static std::string OBJECT_START() { return "OBJECT_START"; }
    inline static std::string ARRAY_START() { return "ARRAY_START"; }
    inline static std::string TUPLE_START() { return "TUPLE_START"; }
    inline static std::string OBJECT_END() { return "OBJECT_END"; }
    inline static std::string ARRAY_END() { return "ARRAY_END"; }
    inline static std::string TUPLE_END() { return "TUPLE_END"; }
    inline static std::string CORE_FUNC() { return "CORE_FUNC"; }
    inline static std::string ANON_FUNC() { return "ANON_FUNC"; }
    inline static std::string FUNC() { return "FUNC"; }
    inline static std::string DECL() { return "DECL"; }
    inline static std::string DECL_METHOD() { return "DECL_METHOD"; }
    inline static std::string CLASS() { return "CLASS"; }
    inline static std::string CHILD() { return "CHILD"; }
    inline static std::string UNEXP() { return "UNEXPECTED"; }
};

#endif // LEXERTYPES_H
