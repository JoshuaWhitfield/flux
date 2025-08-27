#ifndef LEXER_FUNCTIONAL_FULL_H
#define LEXER_FUNCTIONAL_FULL_H

#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <cctype>
#include <regex>

#include "../../../util/headers/debug.h"
#include "../../../util/headers/arrays.h"
#include "../../../util/headers/strings.h"
#include "../../tokens/headers/lexer_token.h"
#include "../../types/headers/lexer_types.h"

#include "../../../functional/control_flow/ffor.h"
#include "../../../functional/control_flow/fwhile.h"
#include "../../../functional/control_flow/fif.h"
#include "../../../functional/control_flow/bincase.h"
#include "../../../functional/operators/structure/ffltr.h"

template<typename T>
inline bool match_object(
    const std::vector<T>& objects,
    std::string& input,
    std::vector<LexerToken>& token_output,
    uint32_t current_line,
    std::function<void(size_t)> consume_chars
) {
    return bincase(
        [&] { return !objects.empty(); },
        [&] {
            auto results = ffor<T, bool>(
                objects,
                [&](Node& node) -> bool {
                    auto& elem = node.getAs<Edge<std::optional<T>>>("elem");
                    return fif(
                        [&] { return input.size() >= elem.get()->text.size(); },
                        [&] {
                            return fif(
                                [&] { return input.substr(0, elem.get()->text.size()) == elem.get()->text; },
                                [&] {
                                    return fif(
                                        [&] {
                                            return (input.size() == elem.get()->text.size()) ||
                                                   (!std::isalnum(static_cast<unsigned char>(input[elem.get()->text.size()])) &&
                                                    input[elem.get()->text.size()] != '_');
                                        },
                                        [&] {
                                            token_output.emplace_back(elem.get()->text, elem.get()->type, current_line);
                                            consume_chars(elem.get()->text.size());
                                            return true;
                                        },
                                        [] { return false; }
                                    );
                                },
                                [] { return false; }
                            );
                        },
                        [] { return false; }
                    );
                }
            );

            auto filtered = ffltr<bool>(results, [](Node&, const bool& r) { return r; });
            return fif([&] { return !filtered.empty(); }, [] { return true; }, [] { return false; });
        },
        [] { return false; }
    );
}

struct Lexer {
    std::string input;
    std::vector<LexerToken> token_output;
    uint32_t current_line;

    inline void add_token(const LexerToken& token) { token_output.push_back(token); }
    inline std::optional<char> get_current() { return input.empty() ? std::nullopt : std::optional<char>(input[0]); }
    inline std::optional<char> consume() {
        auto c = get_current();
        if (c) input = str_slice(input, 1);
        return c;
    }
    inline void consume_chars(size_t n) { input.erase(input.begin(), input.begin() + std::min(n, input.size())); }
    inline void set_input(const std::string& _input) { input = _input; current_line = 0; }

    inline std::optional<LexerToken> tokenize() {
        // Loop while input remains
        auto check_objects = [&](auto& objects) { return match_object(objects, input, token_output, [&](size_t n) { consume_chars(n); }, current_line); };
        struct ObjectCategory { std::string text; std::string type; };
        std::vector<ObjectCategory> declaratives = {
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
        std::vector<ObjectCategory> decl_methods = {
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
        std::vector<ObjectCategory> graphs = {
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
        std::vector<ObjectCategory> core_funcs = {
            {"print", LexerTypes::CORE_FUNC()}, // print statement to std::cout
            {"input", LexerTypes::CORE_FUNC()}, // input statement to std::cin
            {"len",   LexerTypes::CORE_FUNC()}, // length statement returned
            {"type",  LexerTypes::CORE_FUNC()}, // type statement returned
            {"panic", LexerTypes::CORE_FUNC()}  // exit statement with std::exit(0)
        };
        std::vector<ObjectCategory> normalizers = {
            {"constr", LexerTypes::NORMALIZER()}, // class constructor (function in frontend)
            {"not",    LexerTypes::NORMALIZER()}, // evaluates the opposite of the truthiness of its passed parameter
            {"det",    LexerTypes::NORMALIZER()}, // evaluates the direct truthiness of its passed parameter
            {"new",    LexerTypes::NORMALIZER()}, // new normalizes class from definition to a live object instance
        };
        std::vector<ObjectCategory> keywords = {
            {"return",  LexerTypes::KEYWORD()}, // return statement (optional do to assert() variation)
            {"case",    LexerTypes::KEYWORD()}, // case: function call under hood (ffltr), linear switch case
            {"concede", LexerTypes::KEYWORD()}, // in the case selectors placed in params return falsey, you may concede (void return)
         // {"default", LexerTypes::KEYWORD()}  // default case uneccessary, just goes last
        };
        std::vector<ObjectCategory> selectors = {
            {"or",  LexerTypes::SELECTOR()}, // allows 'or' selection of values by truthiness. bitwise with selector(..., true) (function frontend & backend)
            {"and", LexerTypes::SELECTOR()}, // allows 'and' selection of values by truthiness. bitwise with selector(..., true) (function frontend & backend)
            {"xor", LexerTypes::SELECTOR()}, // allows 'xor' selection of values by truthiness. bitwise with selector(..., true) (function frontend & backend)
        };
        std::vector<ObjectCategory> functions = {
            {"func ", LexerTypes::FUNC()}, // standard function declaration
            {"=>", LexerTypes::ANON_FUNC()} // anonymous function declaration
        };
        std::vector<ObjectCategory> identifiers = { // custom
            {"const ", LexerTypes::CONST()}, // constnt identifier declaration
            {"let ", LexerTypes::LET()} // mutable identifier declaration
        };
        std::vector<ObjectCategory> assignments = {
            {"=", LexerTypes::ASSIGN()}, // scope assignment operator
            {":", LexerTypes::ASSIGN()}, // object assignment operator
        };
        std::vector<ObjectCategory> booleans = {
            {"true",  LexerTypes::BOOL()}, // true boolean
            {"false", LexerTypes::BOOL()}, // false boolean
        };
        std::vector<ObjectCategory> classes = {
            {"class", LexerTypes::CLASS()},
        };
        
        return fwhile(
            [&] { return !input.empty(); },
            [&] {
                return bincase(
                    [&] { return get_current().has_value(); },
                    [&] { consume(); return true; },
                    [&] {
                        // skip whitespace
                        return fif([&] { return input[0] == ' ' || input[0] == '\t'; },
                            [&] { consume(); return true; },
                            [&] {
                                // capture newline or statement end
                                return fif([&] { return input[0] == '\n' || input[0] == ';'; },
                                    [&] {
                                        current_line++;
                                        add_token(LexerToken(std::string(1, input[0]), LexerTypes::ENDL(), current_line));
                                        consume(); return true;
                                    },
                                    [&] {
                                        // capture multi-line comments
                                        return fif([&] { return input.size() >= 2 && input[0] == '/' && input[1] == '*'; },
                                            [&] {
                                                size_t i = 2, newline_count = 0;
                                                fwhile(
                                                    [&] { return (i + 1 < input.size()) && (!(input[i] == '*' && input[i + 1] == '/')); }
                                                    [&] {
                                                        return fif(
                                                            [&] { input[i] == '\n'; },
                                                            [&] {
                                                                newline_count++;
                                                                i++;
                                                                return true;
                                                            },
                                                            [&] { return false; },
                                                        )
                                                    }
                                                );
                                                add_token(LexerToken("/*", LexerTypes::COMMENT_START(), current_line));
                                                add_token(LexerToken(str_slice(input, 2, i), LexerTypes::COMMENT(), current_line));
                                                current_line += newline_count;
                                                return fif([&] { ((i + 1) < input.size()); },
                                                    [&] { add_token(LexerToken("*/", LexerTypes::COMMENT_END(), current_line)); consume_chars(i + 2); return true; },
                                                    [&] { consume_chars(input.size()); return false; }
                                                );
                                            },
                                            [&] {
                                                // capture single-line comments
                                                return fif([&] { return input.size() >= 2 && input[0] == '/' && input[1] == '/'; },
                                                    [&] {
                                                        size_t i = 0;
                                                        ffor<size_t>(std::vector<size_t>(input.size()), [&](Node& n) {
                                                            i = n.getAs<Edge<size_t>>("elem").get();
                                                            return fif([&] { return input[i] != '\n'; }, [] { return true; }, [] { return false; });
                                                        });
                                                        add_token(LexerToken("//", LexerTypes::COMMENT_START(), current_line));
                                                        add_token(LexerToken(str_slice(input, 2, i), LexerTypes::COMMENT(), current_line));
                                                        consume_chars(i);
                                                        return true;
                                                    },
                                                    [&] {
                                                        // capture strings
                                                        return fif([&] { return input[0] == '"' || input[0] == '\''; },
                                                            [&] {
                                                                std::smatch match;
                                                                std::regex pattern(R"(\"(.*?)\"|\'(.*?)\')");
                                                                return fif([&] { std::regex_search(input, match, pattern) && match.position(0) == 0; },
                                                                    [&] { 
                                                                        std::string str = match[1].matched ? match[1].str() : match[2].str();
                                                                        add_token(LexerToken(str, LexerTypes::STRING(), current_line));
                                                                        consume_chars(str.size() + 2);
                                                                        return true;
                                                                    },
                                                                    [&] { return false; }
                                                                )
                                                                
                                                            },
                                                            [&] {
                                                                // capture floats and integers
                                                                
                                                                return bincase([&] { return std::isdigit(static_cast<unsigned char>(input[0])) || input[0] == '.'; },
                                                                    [&] { 
                                                                        std::smatch match;
                                                                        std::regex float_pattern(R"(\d*\.\d+)");
                                                                        std::regex int_pattern(R"(\d+)");
                                                                        return fif([&] { return std::regex_search(input, match, float_pattern) && match.position(0) == 0; },
                                                                            [&] { 
                                                                                add_token(LexerToken(match[0].str(), LexerTypes::FLOAT(), current_line));
                                                                                consume_chars(match.length(0));
                                                                                return true;
                                                                            },
                                                                            [&] { 
                                                                                add_token(LexerToken(match[0].str(), LexerTypes::INTEGER(), current_line));
                                                                                consume_chars(match.length(0));
                                                                                return false;
                                                                            }
                                                                            
                                                                        );

                                                                    },
                                                                    [&] {
                                                                        return fif(match_object(
                                                                                booleans,
                                                                                input,
                                                                                token_output,
                                                                                current_line,
                                                                                [&](size_t n) { consume_chars(n); }
                                                                            ),
                                                                            [&] { return true; },
                                                                            [&] { 
                                                                                return fif(
                                                                                    match_object(
                                                                                        assignments,
                                                                                        input,
                                                                                        output_tokens,
                                                                                        current_line,
                                                                                        [&](size_t n) { consume_chars(n); }
                                                                                    ),
                                                                                    [&] { return true; },
                                                                                    [&] {
                                                                                        return fif(
                                                                                            match_object(
                                                                                                class,
                                                                                                input,
                                                                                                output_tokens,
                                                                                                current_line,
                                                                                                [&](size_t n) { consume_chars(n); }
                                                                                            ),
                                                                                            [&] { return true; },
                                                                                            [&] {
                                                                                                return fif(
                                                                                                    match_object(
                                                                                                        selectors,
                                                                                                        input,
                                                                                                        token_output,
                                                                                                        current_line,
                                                                                                        [&](size_t n) { consume_chars(n); }
                                                                                                    ),
                                                                                                    [&] { return true; },
                                                                                                    [&] {
                                                                                                        return fif(
                                                                                                            match_object(
                                                                                                                keywords,
                                                                                                                input,
                                                                                                                output_tokens,
                                                                                                                current_line,
                                                                                                                [&](size_t n) { consume_chars(n); }
                                                                                                            ),
                                                                                                            [&] { return true; },
                                                                                                            [&] {
                                                                                                                return fif(
                                                                                                                    match_object(
                                                                                                                        normalizers,
                                                                                                                        input,
                                                                                                                        token_output,
                                                                                                                        current_line,
                                                                                                                        [&](size_t n) { consume_chars(n); }
                                                                                                                    ),
                                                                                                                    [&] { return true; },
                                                                                                                    [&] {
                                                                                                                        return fif(
                                                                                                                            match_object(
                                                                                                                                core_funcs,
                                                                                                                                input,
                                                                                                                                token_output,
                                                                                                                                current_line,
                                                                                                                                [&](size_t n) { consume_chars(n); }
                                                                                                                            ),
                                                                                                                            [&] { return true; },
                                                                                                                            [&] {
                                                                                                                                return fif(
                                                                                                                                    match_object(
                                                                                                                                        graphs,
                                                                                                                                        input,
                                                                                                                                        token_output,
                                                                                                                                        current_line,
                                                                                                                                        [&](size_t n) { consume_chars(n); }
                                                                                                                                    ),
                                                                                                                                    [&] { return true; },
                                                                                                                                    [&] {
                                                                                                                                        return fif(
                                                                                                                                            match_object(
                                                                                                                                                decl_methods,
                                                                                                                                                input,
                                                                                                                                                token_output,
                                                                                                                                                current_line,
                                                                                                                                                [&](size_t n) { consume_chars(n); }
                                                                                                                                            ),
                                                                                                                                            [&] { return true; },
                                                                                                                                            [&] {
                                                                                                                                                return fif(
                                                                                                                                                    match_object(
                                                                                                                                                        declaratives,
                                                                                                                                                        input,
                                                                                                                                                        token_output,
                                                                                                                                                        current_line,
                                                                                                                                                        [&](size_t n) { consume_chars(n); }
                                                                                                                                                    ),
                                                                                                                                                    [&] { return true; },
                                                                                                                                                    [&] {
                                                                                                                                                        bool matched_function = false;
                                                                                                                                                        ffor(
                                                                                                                                                            functions,
                                                                                                                                                            [&](Node& node) {
                                                                                                                                                                auto elem = node["elem"].get<T>();
                                                                                                                                                                
                                                                                                                                                                bool assert_func_found = fif(
                                                                                                                                                                    [&] { input.substr(0, elem.text.size()) == elem.text; },
                                                                                                                                                                    [&] { return true; },
                                                                                                                                                                    [&] { return false; }
                                                                                                                                                                );

                                                                                                                                                                return bincase(
                                                                                                                                                                    assert_func_found,
                                                                                                                                                                    fif(
                                                                                                                                                                        [&] { return elem.text == "func "; },
                                                                                                                                                                        [&] {
                                                                                                                                                                            std::string after_std_func = input.substr(elem.text.size());
                                                                                                                                                                            std::regex pattern(R"([A-Za-z_][A-Za-z0-9_]*)");
                                                                                                                                                                            std::smatch match;

                                                                                                                                                                            return fif(
                                                                                                                                                                                [&] { return std::regex_search(after_std_func, match, pattern) && match.position(0) == 0; },
                                                                                                                                                                                [&] { 
                                                                                                                                                                                    std::string function = match[0];
                                                                                                                                                                                    add_token(LexerToken(function, elem.type, current_line));

                                                                                                                                                                                    size_t total_matched = elem.text.size() + match.length(0);
                                                                                                                                                                                    consume_chars(total_matched);
                                                                                                                                                                                    matched_function = true;
                                                                                                                                                                                    return matched_function;
                                                                                                                                                                                },
                                                                                                                                                                                [&] { return false; }
                                                                                                                                                                            );
                                                                                                                                                                        },
                                                                                                                                                                        [&] {
                                                                                                                                                                            add_token(LexerToken(elem.text, elem.type, current_line));
                                                                                                                                                                            consume_chars(2);
                                                                                                                                                                            matched_function = true;
                                                                                                                                                                            return matched_function;
                                                                                                                                                                        }
                                                                                                                                                                    ),
                                                                                                                                                                    [&] { return false; }
                                                                                                                                                                );
                                                                                                                                                            }
                                                                                                                                                        );

                                                                                                                                                        return fif(
                                                                                                                                                            [&] { return matched_function; },
                                                                                                                                                            [&] { return true; },
                                                                                                                                                            [&] {
                                                                                                                                                                bool matched_identifier = false;
                                                                                                                                                                ffor(
                                                                                                                                                                    identifiers,
                                                                                                                                                                    [&] {
                                                                                                                                                                        auto elem = node["elem"].get<T>();
                                                                                                                                                                        return bincase(
                                                                                                                                                                            [&] { return input.substr(0, elem.text.size()) == elem.text; },
                                                                                                                                                                            [&] {
                                                                                                                                                                                std::string after_identifier = input.substr(elem.text.size());
                                                                                                                                                                                std::regex pattern(R"([A-Za-z_][A-Za-z0-9_]*)");
                                                                                                                                                                                std::smatch match;

                                                                                                                                                                                return fif(
                                                                                                                                                                                    [&] { std::regex_search(after_identifier, match, pattern) && match.position(0) == 0; },
                                                                                                                                                                                    [&] {
                                                                                                                                                                                        std::string identifier = match[1];
                                                                                                                                                                                        add_token(LexerToken(identifier, elem.type, current_line));
                                                                                                                                                                                        
                                                                                                                                                                                        size_t = total_matched = elem.text.size() + match.length(0);
                                                                                                                                                                                        consume_chars(total_matched);

                                                                                                                                                                                        matched_identifier = true;
                                                                                                                                                                                        return matched_identifier;
                                                                                                                                                                                    },
                                                                                                                                                                                    [&] {return false;}
                                                                                                                                                                                );
                                                                                                                                                                            },
                                                                                                                                                                            [&] { return false; },
                                                                                                                                                                        );
                                                                                                                                                                    }
                                                                                                                                                                );

                                                                                                                                                                return fif(
                                                                                                                                                                    [&] { return matched_identifier; },
                                                                                                                                                                    [&] { return true; },
                                                                                                                                                                    [&] {
                                                                                                                                                                        return fif(
                                                                                                                                                                            // capture commas
                                                                                                                                                                            [&] { input[0] == ','; },
                                                                                                                                                                            [&] {
                                                                                                                                                                                return fif(
                                                                                                                                                                                    [&] { return static_cast<bool>(consume()); },
                                                                                                                                                                                    [&] { 
                                                                                                                                                                                        add_token(LexerToken(std::string(1, *c), LexerTypes::COMMA(), current_line));
                                                                                                                                                                                        return true; 
                                                                                                                                                                                    },
                                                                                                                                                                                    [&] { return false; },
                                                                                                                                                                                );
                                                                                                                                                                            },
                                                                                                                                                                            [&] {
                                                                                                                                                                                return fif(
                                                                                                                                                                                    [&] { input[0] == '['; },
                                                                                                                                                                                    [&] {
                                                                                                                                                                                        return fif(
                                                                                                                                                                                            [&] { return static_cast<bool>(consume()); },
                                                                                                                                                                                            [&] {
                                                                                                                                                                                                add_token(LexerToken(std::string(1, *c), LexerTypes::ARRAY_START(), current_line));
                                                                                                                                                                                                return true;
                                                                                                                                                                                            },
                                                                                                                                                                                            [&] { return false; }
                                                                                                                                                                                        );
                                                                                                                                                                                    },
                                                                                                                                                                                    [&] {
                                                                                                                                                                                        return fif(
                                                                                                                                                                                            [&] { input[0] == ']'; },
                                                                                                                                                                                            [&] {
                                                                                                                                                                                                return fif(
                                                                                                                                                                                                    [&] { return static_cast<bool>(consume()); },
                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                        add_token(LexerToken(std::string(1, *c), LexerTypes::ARRAY_END(), current_line));
                                                                                                                                                                                                        return true;
                                                                                                                                                                                                    },
                                                                                                                                                                                                    [&] { return false; },
                                                                                                                                                                                                );
                                                                                                                                                                                            },
                                                                                                                                                                                            [&] {
                                                                                                                                                                                                return fif(
                                                                                                                                                                                                    [&] { input[0] == '{'; },
                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                            [&] { return static_cast<bool>(consume()); },
                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                add_token(LexerToken(std::string(1, *c), LexerTypes::OBJECT_START(), current_line));
                                                                                                                                                                                                                return true;
                                                                                                                                                                                                            },
                                                                                                                                                                                                            [&] { return false; },
                                                                                                                                                                                                        );
                                                                                                                                                                                                    },
                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                            [&] { input[0] == '}'; },
                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                return fif(
                                                                                                                                                                                                                    [&] { return static_cast<bool>(consume()); },
                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                        add_token(LexerToken(std::string(1, *c), LexerTypes::OBJECT_END(), current_line));
                                                                                                                                                                                                                        return true;
                                                                                                                                                                                                                    },
                                                                                                                                                                                                                    [&] { return false; },
                                                                                                                                                                                                                );
                                                                                                                                                                                                            },
                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                return fif(
                                                                                                                                                                                                                    [&] {return input[0] == '('},
                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                                            [&] { return static_cast<bool>(consume()); },
                                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                                add_token(LexerToken(std::string(1, *c), LexerTypes::TUPLE_START(), current_line));
                                                                                                                                                                                                                                return true;
                                                                                                                                                                                                                            },
                                                                                                                                                                                                                            [&] { return false; },
                                                                                                                                                                                                                        );
                                                                                                                                                                                                                    },
                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                                            [&] { return input[0] == ')'; },
                                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                                return fif(
                                                                                                                                                                                                                                    [&] { return static_cast<bool>(consume()); },
                                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                                        add_token(LexerToken(std::string(1, *c), LexerTypes::TUPLE_END(), current_line));
                                                                                                                                                                                                                                        return true;
                                                                                                                                                                                                                                    },
                                                                                                                                                                                                                                    [&] { return false; },
                                                                                                                                                                                                                                );
                                                                                                                                                                                                                            },
                                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                                return fif(
                                                                                                                                                                                                                                    [&] { return input[0] == '@'; },
                                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                                                            [&] { return static_cast<bool>(consume()); },
                                                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                                                add_token(LexerToken(std::string(1, *c), LexerTypes::REF(), current_line));
                                                                                                                                                                                                                                                return true;
                                                                                                                                                                                                                                            },
                                                                                                                                                                                                                                            [&] { return false; },
                                                                                                                                                                                                                                        );
                                                                                                                                                                                                                                    },
                                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                                        // capture standard unexpected
                                                                                                                                                                                                                                        add_token(LexerToken(split(input, ' ')[0], LexerTypes::UNEXP(), current_line));
                                                                                                                                                                                                                                        return false;
                                                                                                                                                                                                                                    },
                                                                                                                                                                                                                                );
                                                                                                                                                                                                                            },
                                                                                                                                                                                                                        );
                                                                                                                                                                                                                    },
                                                                                                                                                                                                                );
                                                                                                                                                                                                            },
                                                                                                                                                                                                        );
                                                                                                                                                                                                    },
                                                                                                                                                                                                );
                                                                                                                                                                                            }
                                                                                                                                                                                        );
                                                                                                                                                                                    },
                                                                                                                                                                                );
                                                                                                                                                                            },
                                                                                                                                                                        );
                                                                                                                                                                    },
                                                                                                                                                                );
                                                                                                                                                            },
                                                                                                                                                        );
                                                                                                                                                    }
                                                                                                                                                );
                                                                                                                                            }
                                                                                                                                        );
                                                                                                                                    }
                                                                                                                                );
                                                                                                                            }
                                                                                                                        );
                                                                                                                    }
                                                                                                                );
                                                                                                            }
                                                                                                        );
                                                                                                    }
                                                                                                );
                                                                                            }
                                                                                        );
                                                                                    }
                                                                                );
                                                                            }
                                                                        );
                                                                    }
                                                                );
                                                            }
                                                        );
                                                    }
                                                );
                                            }
                                        );
                                    }
                                );
                            }
                        );
                    }
                );
                // capture standard unexpected
                add_token(LexerToken(split(input, ' ')[0], LexerTypes::UNEXP(), current_line));
                break;
            }
        );

        // End-of-file token
        add_token(LexerToken("ENDF", LexerTypes::ENDF(), current_line));
        return std::nullopt;
    }
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
#endif // LEXER_FUNCTIONAL_FULL_H
