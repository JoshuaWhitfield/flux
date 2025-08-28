#ifndef OBJECT_CATEGORIES_H
#define OBJECT_CATEGORIES_H

#include <vector>

#include "../../types/headers/lexer_types.h"

struct ObjectCategory { std::string text; std::string type; };
inline const std::vector<ObjectCategory> declaratives = {
    {"fif",      LexerTypes::DECL()},
    {"ffor",     LexerTypes::DECL()},
    {"fwhile",   LexerTypes::DECL()},
    {"fim",      LexerTypes::DECL()},
    {"feq",      LexerTypes::DECL()},
    {"fneq",     LexerTypes::DECL()},
    {"fgeq",     LexerTypes::DECL()},
    {"fleq",     LexerTypes::DECL()},
    {"fgt",      LexerTypes::DECL()},
    {"flt",      LexerTypes::DECL()},
    {"fpeq",     LexerTypes::DECL()},
    {"fseq",     LexerTypes::DECL()},
    {"fmeq",     LexerTypes::DECL()},
    {"fdeq",     LexerTypes::DECL()},
    {"fmdeq",    LexerTypes::DECL()},
    {"fxeq",     LexerTypes::DECL()},
    {"fmin",     LexerTypes::DECL()},
    {"fadd",     LexerTypes::DECL()},
    {"fmul",     LexerTypes::DECL()},
    {"fdiv",     LexerTypes::DECL()},
    {"fmod",     LexerTypes::DECL()},
    {"fexp",     LexerTypes::DECL()},
    {"finc",     LexerTypes::DECL()},
    {"fdec",     LexerTypes::DECL()},
    {"bincase",  LexerTypes::DECL()},
    {"assert",   LexerTypes::DECL()},
    {"callable", LexerTypes::DECL()},
    {"type",     LexerTypes::DECL()},
    {"fcat",     LexerTypes::DECL()}
};
inline const std::vector<ObjectCategory> decl_methods = {
    {".ffor",  LexerTypes::DECL_METHOD()}, // functor map operation w/ backtracking
    {".ffltr", LexerTypes::DECL_METHOD()}, // functor filter by comparator expression
    {".fred",  LexerTypes::DECL_METHOD()}, // functor reduce values
    {".ffind", LexerTypes::DECL_METHOD()}, // functional find first-match predicate functor
    {".fallm", LexerTypes::DECL_METHOD()}, // functional all match predicate functor
    {".fnonm", LexerTypes::DECL_METHOD()}, // functional none match predicate functor
    {".fsort", LexerTypes::DECL_METHOD()}, // functional reorder by comparator
    {".fmini", LexerTypes::DECL_METHOD()}, // functional get maximum from set w/ functor
    {".fmaxi", LexerTypes::DECL_METHOD()}, // functional get minimum from set w/ functor
    {".last",  LexerTypes::DECL_METHOD()}, // functional get last array index (every type is iterable)
    {".first", LexerTypes::DECL_METHOD()}, // functional get first array index (every type is iteralbe)
    {".pipe",  LexerTypes::DECL_METHOD()}, // functional operate on one value with chain of functions. produce value. requires new iterator
};
inline const std::vector<ObjectCategory> graphs = {
    {"runtime",  LexerTypes::GRAPH_REF()},
    {"express",  LexerTypes::GRAPH_REF()},
    {"deserial", LexerTypes::GRAPH_REF()},
    {"serial",   LexerTypes::GRAPH_REF()},
    {"write",    LexerTypes::GRAPH_REF()},
    /* non forward-facing graphs 
    {"depend", LexerTypes::GRAPH_REF()},
    {"namespace", LexerTypes::GRAPH_REF()},
    {"scope", LexerTypes::GRAPH_REF()}
    */
};
inline const std::vector<ObjectCategory> core_funcs = {
    {"print", LexerTypes::CORE_FUNC()}, // print statement to std::cout
    {"input", LexerTypes::CORE_FUNC()}, // input statement to std::cin
    {"len",   LexerTypes::CORE_FUNC()}, // length statement returned
    {"type",  LexerTypes::CORE_FUNC()}, // type statement returned
    {"panic", LexerTypes::CORE_FUNC()}  // exit statement with std::exit(0)
};
inline const std::vector<ObjectCategory> normalizers = {
    {"constr", LexerTypes::NORMALIZER()}, // class constructor (function in frontend)
    {"not",    LexerTypes::NORMALIZER()}, // evaluates the opposite of the truthiness of its passed parameter
    {"det",    LexerTypes::NORMALIZER()}, // evaluates the direct truthiness of its passed parameter
    {"new",    LexerTypes::NORMALIZER()}, // new normalizes class from definition to a live object instance
};
inline const std::vector<ObjectCategory> keywords = {
    {"return",  LexerTypes::KEYWORD()}, // return statement (optional do to assert() variation)
    {"case",    LexerTypes::KEYWORD()}, // case: function call under hood (ffltr), linear switch case
    {"concede", LexerTypes::KEYWORD()}, // in the case selectors placed in params return falsey, you may concede (void return)
    // {"default", LexerTypes::KEYWORD()}  // default case uneccessary, just goes last
};
inline const std::vector<ObjectCategory> selectors = {
    {"or",  LexerTypes::SELECTOR()}, // allows 'or' selection of values by truthiness. bitwise with selector(..., true) (function frontend & backend)
    {"and", LexerTypes::SELECTOR()}, // allows 'and' selection of values by truthiness. bitwise with selector(..., true) (function frontend & backend)
    {"xor", LexerTypes::SELECTOR()}, // allows 'xor' selection of values by truthiness. bitwise with selector(..., true) (function frontend & backend)
};
inline const std::vector<ObjectCategory> functions = {
    {"func ", LexerTypes::FUNC()}, // standard function declaration
    {"=>", LexerTypes::ANON_FUNC()} // anonymous function declaration
};
inline const std::vector<ObjectCategory> identifiers = { // custom
    {"const ", LexerTypes::CONST()}, // constnt identifier declaration
    {"let ", LexerTypes::LET()} // mutable identifier declaration
};
inline const std::vector<ObjectCategory> assignments = {
    {"=", LexerTypes::ASSIGN()}, // scope assignment operator
    {":", LexerTypes::ASSIGN()}, // object assignment operator
};
inline const std::vector<ObjectCategory> booleans = {
    {"true",  LexerTypes::BOOL()}, // true boolean
    {"false", LexerTypes::BOOL()}, // false boolean
};
inline const std::vector<ObjectCategory> classes = {
    {"class", LexerTypes::CLASS()},
};

#endif // OBJECT_CATEGORIES_H
