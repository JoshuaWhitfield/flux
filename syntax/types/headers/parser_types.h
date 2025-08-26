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
    inline static std::string ANON_FUNC() { return "ANON_FUNCTION"; }

    // ===== Classes & Members =====
    inline static std::string CLASS_DECL() { return "CLASS_DECL"; }
    inline static std::string MEMBER_ACCESS() { return "MEMBER_ACCESS"; }
    inline static std::string MEMBER_DECL() { return "MEMBER_DECL"; }

    // ===== Control Flow =====
    inline static std::string RETURN_STMT() { return "RETURN_STMT"; } // return assert
    inline static std::string CASE_STMT() { return "CASE_STMT"; } // case bincase
    inline static std::string DEFAULT_STMT() { return "DEFAULT_STMT"; } // default
    inline static std::string CONCEDE_STMT() { return "CONCEDE_STMT"; } // concede
    inline static std::string FIF_EXPR() { return "FIF_EXPR"; }
    inline static std::string FWHILE_EXPR() { return "FWHILE_EXPR"; }
    inline static std::string FFOR_EXPR() { return "FFOR_EXPR"; }

    // ===== Declaratives =====
    inline static std::string DECLARATIVE_CALL() { return "DECLARATIVE_CALL"; }
    inline static std::string DECLARATIVE_METHOD() { return "DECLARATIVE_METHOD"; }

    // ===== Operators & Selectors =====
    inline static std::string BINARY_OP() { return "BINARY_OP"; }
    inline static std::string UNARY_OP() { return "UNARY_OP"; }

    // ===== Core Functions =====
    inline static std::string CORE_CALL() { return "CORE_CALL"; } // print input len type panic

    // ===== Selectors =====
    inline static std::string OR() { return "OR"; } // or()
    inline static std::string AND() { return "AND"; } // and()
    inline static std::string XOR() { return "XOR"; } // xor()

    /*

        this allows users to select ecpressions and literals based on
        truthiness. then on the other hand, by passing in a boolean,
        it allows users to run bitwise operations on literals and
        expressions.

    */

    // ===== Normalizers =====
    inline static std::string DET() { return "DETERMINE"; } // det()
    inline static std::string NOT() { return "NOT"; } // not()

    // ===== Misc =====
    inline static std::string REF() { return "REFERENCE"; }
    inline static std::string UNKNOWN() { return "UNKNOWN"; }
    inline static std::string ENDF() { return "ENDF"; }
};

#endif // PARSERTYPES_H
