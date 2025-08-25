#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <cctype>
#include <regex>

#include "../../../util/headers/debug.h" // print
#include "../../../util/headers/arrays.h" // slice join
#include "../../../util/headers/strings.h" // str_slice is_alpha split
#include "../../tokens/headers/lexer_token.h"
#include "../../types/headers/lexer_types.h"

template<typename T>
inline bool match_object(
    const std::vector<T>& objects,
    std::string& input,
    std::vector<LexerToken>& token_output,
    std::function<void(size_t)> consume_chars
) {
    for (auto& obj : objects) {
        if (input.size() < obj.text.size()) continue;

        if (input.substr(0, obj.text.size()) == obj.text) {
            // Properly delimited (space, punctuation, or end)
            if ((input.size() == obj.text.size()) || 
                (!std::isalnum(static_cast<unsigned char>(input[obj.text.size()])) && input[obj.text.size()] != '_')) {
                token_output.push_back(LexerToken(obj.text, obj.type));
                consume_chars(obj.text.size());
                return true;
            }
        }
    }
    return false;
}




template<typename T>
struct Lexer {
    std::string input;
    std::vector<LexerToken> token_output;

    inline void add_token(const LexerToken& token) {
        token_output.push_back(token);
    }

    inline std::optional<char> get_current() {
        if (input.empty()) return std::nullopt;
        return input[0];
    }

    inline std::optional<char> consume() {
        auto currentOpt = get_current();
        if (!currentOpt) return std::nullopt;
        char consumed = currentOpt.value();
        input = str_slice(input, 1);
        return consumed;
    }

    inline void consume_chars(size_t n) {
        if (n > input.size()) n = input.size();
        input.erase(input.begin(), input.begin() + n);
        print("consumed length: " + std::to_string(n));
    }


    inline void set_input(const std::string& _input) {
        input = _input;
    }

    inline std::string get_input() const {
        return input;
    }

    inline std::optional<LexerToken> tokenize() {
        while (!input.empty()) {
            auto currentOpt = get_current();
            if (!currentOpt) break;

            std::string current(1, currentOpt.value());
            if (current.empty()) {
                add_token(LexerToken("null", LexerTypes::UNEXP())); 
                /*

                    before parsing, check if last element 
                    in output_tokens is Token("UNEXPECTED").
                    If it is, throw a runtime error early.
                    error is likely "null value found in script"
                    if the value is std::nullopt
                
                */
                return std::nullopt;
            }

            // Skip whitespace
            //print(current);
            if (input[0] == ' ' || input[0] == '\t') {
                consume();
                continue;
            }
            
            // Handle newline
            char ch = currentOpt.value();
            if (ch == '\n' || ch == ';') {
                add_token(LexerToken(std::string(1, ch), LexerTypes::ENDL()));
                consume();
                continue;
            }
            
            // capture single-line comments
            if (input.size() >= 2 && input[0] == '/' && input[1] == '/') {
                size_t i = 0;
                while (i < input.size() && input[i] != '\n') i++;

                // add tokens
                add_token(LexerToken("//", LexerTypes::COMMENT_START()));
                std::string comment_text = str_slice(input, 0, i);
                //comment_text(input.begin(), input.begin() + i);
                add_token(LexerToken(str_slice(comment_text, 2), LexerTypes::COMMENT()));
                
                // consume properly
                //print(input);
                consume_chars(comment_text.size()); // i is index of '\n', i.e., total chars from 0 → i-1? 
                //exit();

                continue;
            }

            // capture multi-line comments
            if (input.size() >= 2 && input[0] == '/' && input[1] == '*') {
                size_t i = 2;
                while (i + 1 < input.size() && !(input[i] == '*' && input[i + 1] == '/')) i++;

                std::string comment_text(input.begin() + 2, (i + 1 < input.size()) ? input.begin() + i : input.end());
                add_token(LexerToken("/*", LexerTypes::COMMENT_START()));
                add_token(LexerToken(comment_text, LexerTypes::COMMENT()));
                
                if (i + 1 < input.size()) {
                    add_token(LexerToken("*/", LexerTypes::COMMENT_END()));
                    consume_chars(i + 2); // consume '/*content*/'
                } else {
                    consume_chars(input.size()); // unterminated
                }
            }

            // capture strings
            //print(input[0]);
            if (input[0] == '\'' || input[0] == '"') {
                std::smatch match;
                std::regex pattern(R"(\"(.*?)\"|\'(.*?)\')");
                
                if (std::regex_search(input, match, pattern) && match.position(0) == 0) {
                    std::string str;
                    
                    if (match[1].matched) {
                        str = match[1].str();
                    } else if (match[2].matched) {
                        str = match[2].str();
                    }
                    
                    add_token(LexerToken(str, LexerTypes::STRING()));
                    consume_chars(str.size() + 2);
                    //print("size: " + std::to_string(str.size()) + "; text = " + str);
                    continue;
                }
            }
            
            // capture floating points
            if (std::isdigit(static_cast<unsigned char>(input[0])) || input[0] == '.') {
                std::smatch match;
                std::regex pattern(R"(\d*\.\d+)");
                
                if (std::regex_search(input, match, pattern) && match.position(0) == 0) {
                    add_token(LexerToken(match[0].str(), LexerTypes::FLOAT()));
                    consume_chars(match.length(0));
                    continue;
                }
            }
            
            // capture integers
            if (std::isdigit(static_cast<unsigned char>(input[0]))) {
                std::smatch match; 
                std::regex pattern(R"(\d+)");
                
                if (std::regex_search(input, match, pattern) && match.position(0) == 0) {
                    add_token(LexerToken(match[0].str(), LexerTypes::INTEGER()));
                    consume_chars(match.length(0));
                    continue;
                }
            }
            
            // capture identifiers
            struct Identifier {
                std::string text;
                std::string type;
                Identifier(const std::string& _text, const std::string& _type) 
                    : text(_text), type(_type) {}
            };
            
            std::vector<Identifier> identifiers = {
                Identifier("const ", LexerTypes::CONST()),
                Identifier("let ", LexerTypes::LET())
            };
            
            
            bool matched_identifier = false;
            for (auto& id : identifiers) {
                if (input.substr(0, id.text.size()) == id.text) {
                    std::string after_identifier = input.substr(id.text.size());
                    
                    std::regex pattern(R"(([A-Za-z_][A-Za-z0-9_]*))");
                    std::smatch match;
                    
                    if (std::regex_search(after_identifier, match, pattern) && match.position(0) == 0) {
                        std::string identifier = match[1];
                        add_token(LexerToken(identifier, id.type));
                        
                        size_t total_matched = id.text.size() + match.length(0);
                        consume_chars(total_matched);
                        
                        matched_identifier = true;
                        break; // identifier type handled, move to next iteration
                    }
                }
            }
            
            if (matched_identifier) continue;
            
            // capture data structures: array, object, tuple
            if (input[0] == ',') {
                if (auto c = consume()) {
                    add_token(LexerToken(std::string(1, *c), LexerTypes::COMMA()));
                }
                continue;
            }
            
            if (input[0] == '[') {
                if (auto c = consume()) {
                    add_token(LexerToken(std::string(1, *c), LexerTypes::ARRAY_START()));
                }
                continue;
            }
            
            if (input[0] == ']') {
                if (auto c = consume()) {
                    add_token(LexerToken(std::string(1, *c), LexerTypes::ARRAY_END()));
                }
                continue;
            }
            
            if (input[0] == '{') {
                if (auto c = consume()) {
                    add_token(LexerToken(std::string(1, *c), LexerTypes::OBJECT_START()));
                }
                continue;
            }
            
            if (input[0] == '}') {
                if (auto c = consume()) {
                    add_token(LexerToken(std::string(1, *c), LexerTypes::OBJECT_END()));
                }
                continue;
            }
            
            if (input[0] == '(') {
                if (auto c = consume()) {
                    add_token(LexerToken(std::string(1, *c), LexerTypes::TUPLE_START()));
                }
                continue;
            }
            
            if (input[0] == ')') {
                if (auto c = consume()) {
                    add_token(LexerToken(std::string(1, *c), LexerTypes::TUPLE_END()));
                }
                continue;
            }

            // capture references
            if (input[0] == '@') {
                if (auto c = consume()) {
                    add_token(LexerToken(std::string(1, *c), LexerTypes::REF()));
                }
                continue;
            }

            
            // capture standard function
            struct Function {
                std::string text;
                std::string type;
                Function(const std::string& _text, const std::string& _type)
                : text(_text), type(_type) {}
            };
            
            std::vector<Function> functions = {
                Function("func ", LexerTypes::FUNC()),
                Function("=>", LexerTypes::ANON_FUNC())
            };
            
            bool matched_function = false;
            for (auto& fn : functions) {
                if (input.substr(0, fn.text.size()) != fn.text) continue; // skip to next function type
                
                if (input.substr(0, fn.text.size()) == fn.text) {
                    if (fn.text == "func ") {
                        std::string after_std_func = input.substr(fn.text.size());
                        
                        std::regex pattern(R"(([A-Za-z_][A-Za-z0-9_]*))");
                        std::smatch match;
                        
                        if (std::regex_search(after_std_func, match, pattern) && match.position(0) == 0){
                            std::string function = match[1];
                            add_token(LexerToken(function, fn.type));
                            
                            size_t total_matched = fn.text.size() + match.length(0);
                            consume_chars(total_matched);
                            matched_function = true;
                            break; // standard function type handled, move to next iteration
                        }
                    }
                    
                    add_token(LexerToken(fn.text, fn.type));
                    consume_chars(2);
                    matched_function = true;
                    break; // anon function type handled, move to next iteration
                }
            }
            
            if (matched_function) continue;
            
            // capture keywords
            struct Keyword {
                std::string text;
                std::string type;
                
                Keyword(const std::string& _text, const std::string& _type)
                : text(_text), type(_type) {}
            };
            
            std::vector<Keyword> keywords = {
                Keyword("return",  LexerTypes::KEYWORD()),
                Keyword("case",    LexerTypes::KEYWORD()),
                Keyword("concede", LexerTypes::KEYWORD()),
                Keyword("default", LexerTypes::KEYWORD()),
            };
            
            if (match_object<Keyword>(
                keywords,
                input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;
            
            // capture core functions
            struct CoreFunction {
                std::string text;
                std::string type;
                
                CoreFunction(const std::string& _text, const std::string& _type)
                : text(_text), type(_type) {}
            };
            
            std::vector<CoreFunction> core_functions = {
                CoreFunction("print", LexerTypes::CORE_FUNC()),
                CoreFunction("input", LexerTypes::CORE_FUNC()),
                CoreFunction("len",   LexerTypes::CORE_FUNC()),
                CoreFunction("type",  LexerTypes::CORE_FUNC()),
                CoreFunction("panic", LexerTypes::CORE_FUNC())
            };
            
            if (match_object<CoreFunction>(
                core_functions,
                input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;
            
            // capture declaratives
            struct Declarative {
                std::string text;
                std::string type;
                
                Declarative(const std::string& _text, const std::string& _type)
                : text(_text), type(_type) {}
            };
            
            std::vector<Declarative> declaratives = {
                // control flow
                Declarative("fif",      LexerTypes::DECL()), // functionally optimized if block
                Declarative("fwhile",   LexerTypes::DECL()), // tco optimized recursive while loop
                Declarative("ffor",     LexerTypes::DECL()), // function version of .ffor method attached to every type accept maps. optional usage
                Declarative("fim",      LexerTypes::DECL()), // higher order function that returns its callback function for storage or computation
                // comparisons
                Declarative("feq",      LexerTypes::DECL()), // ==
                Declarative("fneq",     LexerTypes::DECL()), // !=
                Declarative("fgeq",     LexerTypes::DECL()), // >= 
                Declarative("fleq",     LexerTypes::DECL()), // <=
                Declarative("fgt",      LexerTypes::DECL()), // >
                Declarative("flt",      LexerTypes::DECL()), // <
                Declarative("fpeq",     LexerTypes::DECL()), // +=
                Declarative("fseq",     LexerTypes::DECL()), // -=
                Declarative("fmeq",     LexerTypes::DECL()), // *=
                Declarative("fdeq",     LexerTypes::DECL()), // /=
                Declarative("fmdeq",    LexerTypes::DECL()), // %=
                Declarative("fxeq",     LexerTypes::DECL()), // ^=
                // math operators
                Declarative("fmin",     LexerTypes::DECL()), // -
                Declarative("fadd",     LexerTypes::DECL()), // +
                Declarative("fmul",     LexerTypes::DECL()), // *
                Declarative("fdiv",     LexerTypes::DECL()), // /
                Declarative("fmod",     LexerTypes::DECL()), // %
                Declarative("fexp",     LexerTypes::DECL()), // ^
                Declarative("finc",     LexerTypes::DECL()), // ++
                Declarative("fdec",     LexerTypes::DECL()), // --
                // switch cases
                Declarative("bincase",  LexerTypes::DECL()), // binary swith case
                Declarative("concat",   LexerTypes::DECL()), // type + type
                // assertions
                Declarative("assert",   LexerTypes::DECL()), // if == then return; storeable return value; return on called line outside of internal runtime scope
                Declarative("callable", LexerTypes::DECL()), //
                Declarative("type",     LexerTypes::DECL()), //
                
                
                /*
                
                fmin, fadd, fmul, fdiv all do the following
                they normalize non integer values to truthy and falsey and proceed with their operation.
                this allows you to manipulate integers and floats with other types as if they are viable.
                they equate to 1 and 0 only, which allows for truthiness checks on information without det and not.
                
                */
            };
            
            if (match_object<Declarative>(
                declaratives,
                input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;
            
            // capture declarative methods
            struct DeclarativeMethod {
                std::string text;
                std::string type;
                
                DeclarativeMethod(const std::string& _text, const std::string& _type)
                : text(_text), type(_type) {}
            };
            
            std::vector<DeclarativeMethod> declarative_methods = {
                DeclarativeMethod(".ffor",   LexerTypes::DECL_METHOD()),
                DeclarativeMethod(".concat", LexerTypes::DECL_METHOD())
            };
            
            if (match_object<DeclarativeMethod>(
                declarative_methods,
                input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;
            
            // capture selectors
            struct Selector {
                std::string text;
                std::string type;
                
                Selector(const std::string& _text, const std::string& _type)
                : text(_text), type(_type) {}
            };
            
            std::vector<Selector> selectors = {
                Selector("and", LexerTypes::SELECTOR()), // selects all values that were true, if falsey detected, [] is returned
                Selector("or",  LexerTypes::SELECTOR()), // selects whichever values are truthy only between two thruth parameters of any type
                Selector("xor", LexerTypes::SELECTOR()), // selects whichever value is true between two truth parameters of opposite type
            };
            
            if (match_object<Selector>(
                selectors,
                input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;
            
            // capture normalizers
            struct Normalizer {
                std::string text;
                std::string type; 
                
                Normalizer(const std::string& _text, const std::string& _type) 
                : text(_text), type(_type) {}
            };
            
            std::vector<Normalizer> normalizers = {
                Normalizer("not", LexerTypes::NORMALIZER()), // normilizes parameter to opposite of value's truthy or falsey properties in bool form
                Normalizer("det", LexerTypes::NORMALIZER())  // normalizes parameter to direct representation of value's truthy or falsey properties in bool form
            };
            
            if (match_object<Normalizer>(
                normalizers,
                input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;
            
            // capture namespaces
            //print("input current: " + std::to_string(static_cast<unsigned char>(input[0])));
            if (std::isalpha(static_cast<unsigned char>(input[0]))) {
                std::smatch match;
                std::regex pattern(R"(&?[A-Za-z_][A-Za-z0-9_]*)");
                if (std::regex_search(input, match, pattern) && match.position(0) == 0) {
                    //print("match pos: " + std::to_string(match.position(0)));
                    std::string ns_name = match[0].str();
                    add_token(LexerToken(ns_name, LexerTypes::NAMESPACE()));
                    consume_chars(ns_name.size());
                    //print("namespace: " + ns_name);
                    continue;
                }
            }

            // capture assignment
            struct Assignment {
                std::string text;
                std::string type;

                Assignment(const std::string& _text, const std::string& _type)
                    : text(_text), type(_type) {}
            };

            std::vector<Assignment> assignments {
                Assignment("=", LexerTypes::ASSIGN()), // for standard assignments
                Assignment(":", LexerTypes::ASSIGN()) // for object assignments
            };

            if (match_object<Assignment>(
                assignments,
                input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) {
                continue;
            }

            //capture children
            if (input[0] == '.') {
                std::regex pattern(R"(\.([A-Za-z_][A-Za-z0-9_]*))");
                std::smatch match;

                if (std::regex_search(input, match, pattern) && match.position(0) == 0) {
                    add_token(LexerToken(match[1], LexerTypes::CHILD()));
                    consume_chars(match.length(0));
                    continue;
                }
            }

            //capture classes
            struct Class {
                std::string text;
                std::string type;

                Class(const std::string& _text, const std::string& _type)
                    : text(_text), type(_type) {}
            };

            std::vector<Class> classes = {
                Class("class", LexerTypes::CLASS())
            };

            if (match_object<Class>(
                classes,
                input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;


            print("checkpoint unexpected");

            // capture standard unexpected
            add_token(LexerToken(split(input, ' ')[0], LexerTypes::UNEXP()));
            break;

            /*
            
                if the Token("UNEXPECTED") contains an empty string as
                a value, then it is a traditional unexpected token. 
                it is handled variably and dynamically depending on 
                where it is in the parsing process.
            
            */

        }

        // capture end-of-file
        add_token(LexerToken("ENDF", LexerTypes::ENDF()));
        return std::nullopt;
    }
};

#endif // LEXER_H
