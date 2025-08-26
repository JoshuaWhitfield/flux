#ifndef PARSERTYPES_H
#define PARSERTYPES_H

#pragma once
#include <string>

struct ParserTypes {
    // ===== Literals & Namespaces =====
    inline static std::string NAMESPACE() { return "NAMESPACE"; }
    inline static std::string STRING_LITERAL() { return "STRING_LITERAL"; }
    inline static std::string INTEGER_LITERAL() { return "INTEGER_LITERAL"; }
    inline static std::string FLOAT_LITERAL() { return "FLOAT_LITERAL"; }
    inline static std::string BOOLEAN_LITERAL() { return "BOOLEAN_LITERAL"; }

    // ===== Data Structures =====
    inline static std::string ARRAY_LITERAL() { return "ARRAY_LITERAL"; }
    inline static std::string OBJECT_LITERAL() { return "OBJECT_LITERAL"; }
    inline static std::string TUPLE_LITERAL() { return "TUPLE_LITERAL"; }

    // ===== Declarations & Assignment =====
    inline static std::string CONST_DECL() { return "CONST_DECL"; }
    inline static std::string LET_DECL() { return "LET_DECL"; }
    inline static std::string ASSIGNMENT() { return "ASSIGNMENT"; }

    // ===== Functions =====
    inline static std::string FUNCTION_DECL() { return "FUNCTION_DECL"; }
    inline static std::string FUNCTION_CALL() { return "FUNCTION_CALL"; }
    inline static std::string ANON_FUNCTION() { return "ANON_FUNCTION"; }

    // ===== Classes & Members =====
    inline static std::string CLASS_DECL() { return "CLASS_DECL"; }
    inline static std::string MEMBER_ACCESS() { return "MEMBER_ACCESS"; }
    inline static std::string MEMBER_DECL() { return "MEMBER_DECL"; }

    // ===== Control Flow =====
    inline static std::string RETURN_STMT() { return "RETURN_STMT"; }
    inline static std::string CASE_STMT() { return "CASE_STMT"; }
    inline static std::string DEFAULT_STMT() { return "DEFAULT_STMT"; }
    inline static std::string CONCEDE_STMT() { return "CONCEDE_STMT"; }
    inline static std::string IF_EXPR() { return "IF_EXPR"; }
    inline static std::string WHILE_EXPR() { return "WHILE_EXPR"; }

    // ===== Declaratives =====
    inline static std::string DECLARATIVE_CALL() { return "DECLARATIVE_CALL"; }
    inline static std::string DECLARATIVE_METHOD() { return "DECLARATIVE_METHOD"; }

    // ===== Operators & Selectors =====
    inline static std::string BINARY_OP() { return "BINARY_OP"; }
    inline static std::string UNARY_OP() { return "UNARY_OP"; }

    // ===== Core Functions =====
    inline static std::string CORE_CALL() { return "CORE_CALL"; }

    // ===== Misc =====
    inline static std::string REFERENCE() { return "REFERENCE"; }
    inline static std::string UNKNOWN() { return "UNKNOWN"; }
    inline static std::string END_OF_FILE() { return "EOF"; }
};

#endif // PARSERTYPES_H
