#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <cctype>
#include <regex>

#include "../../util/arrays.h" // slice join
#include "../../util/strings.h" // str_slice is_alpha
#include "../tokens/headers/lexer_token.h"
#include "../types/headers/lexer_types.h"

template<typename T>
bool match_object(
    const std::vector<T>& objects, 
    std::string& joined_input, 
    std::vector<LexerToken>& token_output, 
    std::function<void(size_t)> consume_chars
) {
    for (auto& obj : objects) {
        if (joined_input.substr(0, text.size()) == text) {
            char next_char = joined_input[text.size()];
            if (std::isspace(next_char) || next_char == "(" || 
                next_char == ";" || next_char == "\0" || next_char == "\n"
            ) {
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
    std::vector<std::string> input;
    std::string joined_input;
    std::vector<LexerToken> token_output;

    void add_token(const LexerToken& token) {
        token_output.push_back(token);
    }

    std::optional<std::string> get_current() {
        if (input.empty()) return std::nullopt;
        return input[0];
    }

    std::optional<std::string> consume() {
        auto currentOpt = get_current();
        if (!currentOpt) return std::nullopt;
        std::string consumed = currentOpt.value();
        input = slice(input, 1);
        joined_input = str_slice(joined_input, 1);
        return consumed;
    }

    void consume_chars(size_t n) {
        if (n > input.size()) n = input.size();
        input.erase(input.begin(), input.begin() + n);
        joined_input.erase(joined_input.begin(), joined_input.begin() + n);
    }

    void set_input(const std::vector<std::string>& _input) {
        input = _input;
        joined_input = join(_input);
    }

    std::vector<std::string> get_input() const {
        return input;
    }

    std::optional<LexerToken> tokenize() {
        while (!input.empty()) {
            auto currentOpt = get_current();
            if (!currentOpt) break;

            std::string current = currentOpt.value();
            if (current.empty()) {
                add_token(LexerToken(std::nullopt, LexerTypes.UNEXP())); 
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
            if (std::isspace(current[0])) {
                consume();
                continue;
            }

            // Handle newline
            if (current == "\\n" || current == ";") {
                add_token(LexerToken(consume(), TokenTypes.ENDL()));
                continue;
            }

            // capture identifiers
            struct Identifier {
                std::string text;
                std::string type;
                Identifier(const std::string& _text, const std::string& _type) 
                    : text(_text), type(_type) {}
            };
            
            std::vector<Identifier> identifiers = {
                Identifier("const ", TokenTypes.CONST()),
                Identifier("let ", TokenTypes.LET())
            };
            
            bool matched_identifier = false;
            for (auto& id : identifiers) {
                if (joined_input.substr(0, id.text.size()) == id.text) {
                    std::string after_identifier = joined_input.substr(id.text.size());

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

            // capture namespaces
            if (std::isalpha(static_cast<unsigned char>(input[0]))) {
                std::smatch match;
                std::regex pattern(R"(&?[A-Za-z_][A-Za-z0-9_]*)");

                if (std::regex_search(joined_input, match, pattern) && match.position(0) == 0) {
                    std::string ns_name = match[0].str();
                    add_token(LexerToken(ns_name, LexerTypes.NAMESPACE()));
                    consume_chars(ns_name.size());
                    continue;
                }
            }

            // capture strings
            if (input[0] == "'" || input[0] == '"') {
                std::smatch match;
                std::regex pattern(R"(\"(.*?)\"|\'(.*?)\')");
                
                if (std::regex_search(joined_input, match, pattern) && match.position(0) == 0) {
                    std::string str;
                    
                    if (match[1].matched) {
                        str = match[1].str();
                    } else if (match[2].matched) {
                        str = match[2].str();
                    }

                    add_token(LexerToken(str, LexerTypes.STRING()));
                    consume_chars(str.size() + 2);
                    continue;
                }
            }
            
            // capture floating points
            if (std::isdigit(static_cast<unsigned char>(input[0])) || input[0] == ".") {
                std::smatch match;
                std::regex pattern(R"(\d*\.\d+)");

                if (std::regex_search(joined_input, match, pattern) && match.position(0) == 0) {
                    float floating_pt = std::stof(match[0].str()); // typecast from string to float
                    add_token(LexerToken(floating_pt, LexerTypes.FLOAT()));
                    consume_chars(match.length(0));
                    continue;
                }
            }

            // capture integers
            if (std::isdigit(static_cast<unsigned char>(input[0]))) {
                std::smatch match; 
                std::regex pattern(R"(\d+)");

                if (std::regex_search(joined_input, match, pattern) && match.position(0) == 0) {
                    int integer = std::stoi(match[0].str()); // typecast from string to integer
                    add_token(LexerToken(integer, LexerTypes.INTEGER()));
                    consume_chars(match.length(0));
                    continue;
                }
            }

            // capture data structures: array, object, tuple
            if (input[0] == ",") {
                add_token(LexerToken(consume(), LexerTypes.COMMA()));
                continue;
            }

            if (input[0] == "[") {
                add_token(LexerToken(consume(), LexerTypes.ARRAY_START()));
                continue;
            }

            if (input[0] == "]") {
                add_token(LexerToken(consume(), LexerTypes.ARRAY_END()));
                continue;
            }

            if (input[0] == "{") {
                add_token(LexerToken(consume(), LexerTypes.OBJECT_START()));
                continue;
            }

            if (input[0] == "}") {
                add_token(LexerToken(consume(), LexerTypes.OBJECT_END()));
                continue;
            }

            if (input[0] == "(") {
                add_token(LexerToken(consume(), LexerTypes.TUPLE_START()));
                continue;
            }

            if (input[0] == ")") {
                add_token(LexerToken(consume(), LexerTypes.TUPLE_END()));
                continue;
            }

            // capture standard function
            struct Function {
                std::string text;
                std::string type;
                Function(const std::string& _text, const std::string& _type)
                    : text(_text), type(_type) {}
            }

            std::vector<Function> functions = {
                Function("func ", TokenTypes.FUNC()),
                Function("=>", TokenTypes.ANON_FUNC())
            }

            bool matched_function = false;
            for (auto& fn : functions) {
                if (joined_input.substr(0, fn.text.size()) != fn.text) continue; // skip to next function type

                if (joined_input.substr(0, fn.text.size()) == fn.text) {
                    if (fn.text == "func ") {
                        std::string after_std_func = joined_input.substr(fn.text.size());

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
                    matched_function = true;
                    break; // anon function type handled, move to next iteration
                }
            }

            if (matchedFunction) continue;

            // capture keywords
            struct Keyword {
                std::string text;
                std::string type;

                Keyword(const std::string& _text, const std::string& _type)
                    : text(_text), type(_type) {}
            };

            std::vector<Keyword> keywords = {
                Keyword("return",  LexerTypes.KEYWORD()),
                Keyword("case",    LexerTypes.KEYWORD()),
                Keyword("concede", LexerTypes.KEYWORD()),
                Keyword("default", LexerTypes.KEYWORD()),
            }

            if (matched_object<Keyword>(
                keywords,
                joined_input,
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
                CoreFunction("print", LexerTypes.CORE_FUNC()),
                CoreFunction("input", LexerTypes.CORE_FUNC()),
                CoreFunction("len",   LexerTypes.CORE_FUNC()),
                CoreFunction("type",  LexerTypes.CORE_FUNC()),
                CoreFunction("panic", LexerTypes.CORE_FUNC())
            }

            if (matched_object<CoreFunction>(
                core_functions,
                joined_input,
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
                Declarative("fif",      LexerTypes.DECL()), // functionally optimized if block
                Declarative("fwhile",   LexerTypes.DECL()), // tco optimized recursive while loop
                Declarative("feq",      LexerTypes.DECL()), // ==
                Declarative("fgeq",     LexerTypes.DECL()), // >= 
                Declarative("fleq",     LexerTypes.DECL()), // <=
                Declarative("bincase",  LexerTypes.DECL()), // binary swith case
                Declarative("fgt",      LexerTypes.DECL()), // >
                Declarative("flt",      LexerTypes.DECL()), // <
                Declarative("concat",   LexerTypes.DECL()), // type + type
                Declarative("fpeq",     LexerTypes.DECL()), // +=
                Declarative("fmeq",     LexerTypes.DECL()), // -=
            };

            if (matched_object<Declarative>(
                declaratives,
                joined_input,
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
                DeclarativeMethod(".ffor",   LexerTypes.DECLARATIVE_METHOD()),
                DeclarativeMethod(".concat", LexerTypes.DECLARATIVE_METHOD())
            };

            if (matched_object<DeclarativeMethod>(
                declarative_methods,
                joined_input,
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
                Selector("and", LexerTypes.SELECTOR()), // selects all values that were true, if falsey detected, [] is returned
                Selector("or",  LexerTypes.SELECTOR()), // selects whichever values are truthy only between two thruth parameters of any type
                Selector("xor", LexerTypes.SELECTOR()), // selects whichever value is true between two truth parameters of opposite type
            };

            if (matched_object<Selector>(
                selectors,
                joined_input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;

            // capture normalizers
            struct Normalizer {
                std::string text;
                std::string type; 

                Normalizer(const std::string& _text, const std::string& _type) 
                    : text(_text), type(_type) {}
            }

            std::vector<Normalizer> normalizers = {
                Normalizer("not", LexerTypes.NORMALIZER()), // normilizes parameter to opposite of value's truthy or falsey properties in bool form
                Normalizer("det", LexerTypes.NORMALIZER())  // normalizes parameter to direct representation of value's truthy or falsey properties in bool form
            }

            if (matched_object<Normalizer>(
                normalizers,
                joined_input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;

            // capture assignment
            struct Assignment {
                std::string text;
                std::string type;

                Assignment(const std::string& _text, const std::string& _type)
                    : text(_text), type(_type) {}
            }

            std::vector<Assignment> assignments {
                Assignment("=", LexerType.ASSIGN()), // for standard assignments
                Assignment(":", LexerType.ASSIGN()) // for object assignments
            }

            if (matched_object<Assignment>(
                assignments,
                joined_input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;

            //capture children
            if (input[0] == ".") {
                std::regex pattern(R"(\.([A-Za-z_][A-Za-z0-9_]*))");
                std::smatch match;

                if (std::regex_search(joined_input, match, pattern) && match.position(0) == 0) {
                    add_token(LexerToken(match[1], LexerTypes.CHILD()));
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
            }

            std::vector<Class> classes = {
                Class("class", LexerTypes.CLASS())
            }

            if (matched_object<Class>(
                classes,
                joined_input,
                token_output,
                [&](size_t n) { consume_chars(n); }
            )) continue;


            // capture standard unexpected
            add_token(LexerToken(split(joined_input, " ")[0], TokenTypes.UNEXP()));
            break;

            /*
            
                if the Token("UNEXPECTED") contains an empty string as
                a value, then it is a traditional unexpected token. 
                it is handled variably and dynamically depending on 
                where it is in the parsing process.
            
            */

        }

        // capture end-of-file
        add_token(LexerToken("", TokenTypes.ENDF()));
        return std::nullopt;
    }
};
