#ifndef LEXER_FUNCTIONAL_FULL_H
#define LEXER_FUNCTIONAL_FULL_H

#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <cctype>
#include <regex>
#include <numeric>

#include "../../../util/headers/debug.h" // print exit
#include "../../../util/headers/arrays.h" // slice join
#include "../../../util/headers/strings.h" // str_slice split
#include "../../tokens/headers/lexer_token.h" // LexerToken
#include "../../types/headers/lexer_types.h" // LexerTypes
#include "../../lexer/headers/token_constants.h" // ObjectCategory ...

#include "../../../functional/control_flow/ffor.h" // ffor extract_edge
#include "../../../functional/control_flow/fwhile.h" // fwhile 
#include "../../../functional/control_flow/fif.h" // fif 
#include "../../../functional/control_flow/bincase.h" // bincase
#include "../../../functional/operators/structure/ffltr.h" // ffltr
#include "../../../functional/models/edge.h" // Edge

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
                    T elem;
                    auto& elemEdge = extract_edge<Edge, std::optional<T>>(node, "elem"); // Edge<std::optional<T>>&
                    auto& elemOpt  = elemEdge.get();                                      // std::optional<T>&
                    fif(
                        [&] { return elemOpt.has_value(); },
                        [&] {
                            elem = elemOpt.value();
                            return true;
                        },
                        [&] { return false; }
                    );

                    return fif(
                        [&] { return input.size() >= elem.text.size(); },
                        [&] {
                            return fif(
                                [&] { return input.substr(0, elem.text.size()) == elem.text; },
                                [&] {
                                    return fif(
                                        [&] {
                                            return (input.size() == elem.text.size()) ||
                                                   (!std::isalnum(static_cast<unsigned char>(input[elem.text.size()])) &&
                                                    input[elem.text.size()] != '_');
                                        },
                                        [&] {
                                            token_output.emplace_back(elem.text, elem.type, current_line);
                                            consume_chars(elem.text.size());
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

template<typename T>
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
        // auto check_objects = [&](auto& objects) { return match_object(objects, input, token_output, [&](size_t n) { consume_chars(n); }, current_line); };

        return fwhile<bool, bool>(
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
                                                fwhile<bool, bool>(
                                                    [&] { return (i + 1 < input.size()) && (!(input[i] == '*' && input[i + 1] == '/')); },
                                                    [&] {
                                                        return fif(
                                                            [&] { return input[i] == '\n'; },
                                                            [&] {
                                                                newline_count++;
                                                                i++;
                                                                return true;
                                                            },
                                                            [&] { return false; }
                                                        );
                                                    }
                                                );
                                                add_token(LexerToken("/*", LexerTypes::COMMENT_START(), current_line));
                                                add_token(LexerToken(str_slice(input, 2, i), LexerTypes::COMMENT(), current_line));
                                                current_line += newline_count;
                                                return fif([&] { return ((i + 1) < input.size()); },
                                                    [&] { add_token(LexerToken("*/", LexerTypes::COMMENT_END(), current_line)); consume_chars(i + 2); return true; },
                                                    [&] { consume_chars(input.size()); return false; }
                                                );
                                            },
                                            [&] {
                                                // capture single-line comments
                                                return fif([&] { return input.size() >= 2 && input[0] == '/' && input[1] == '/'; },
                                                    [&] {
                                                        int i;
                                                        std::vector<size_t> indices(input.size() - 2); // -2 because we start at position 2
                                                        std::iota(indices.begin(), indices.end(), 2);  // Fill with values 2, 3, 4, ...

                                                        ffor<size_t, bool>(indices, [&](Node& node) -> bool {
                                                            auto& current_pos_edge = extract_edge<Edge, int>(node, "index"); // Edge<int>&
                                                            i = current_pos_edge.get(); // unwrap Edge<int>

                                                            if (input[i] == '\n') {
                                                                node.getAs<Edge<bool>>("end").set(true);
                                                                return true;
                                                            }
                                                            return false;
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
                                                                );
                                                                
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
                                                                                        token_output,
                                                                                        current_line,
                                                                                        [&](size_t n) { consume_chars(n); }
                                                                                    ),
                                                                                    [&] { return true; },
                                                                                    [&] {
                                                                                        return fif(
                                                                                            match_object(
                                                                                                classes,
                                                                                                input,
                                                                                                token_output,
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
                                                                                                                token_output,
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
                                                                                                                                                        ffor<T, bool>(
                                                                                                                                                            functions,
                                                                                                                                                            [&](Node& node) {
                                                                                                                                                                T elem;
                                                                                                                                                                auto& elemEdge = extract_edge<Edge, std::optional<T>>(node, "elem"); // Edge<std::optional<T>>&
                                                                                                                                                                auto& elemOpt  = elemEdge.get();                                      // std::optional<T>&
                                                                                                                                                                fif(
                                                                                                                                                                    [&] { return elemOpt.has_value(); },
                                                                                                                                                                    [&] {
                                                                                                                                                                        elem = elemOpt.value();
                                                                                                                                                                        return true;
                                                                                                                                                                    },
                                                                                                                                                                    [&] { return false; }
                                                                                                                                                                );

                                                                                                                                                                bool assert_func_found = fif(
                                                                                                                                                                    [&] { return input.substr(0, elem.text.size()) == elem.text; },
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
                                                                                                                                                                ffor<T, bool>(
                                                                                                                                                                    identifiers,
                                                                                                                                                                    [&](Node& node) {
                                                                                                                                                                        T elem;
                                                                                                                                                                        auto& elemEdge = extract_edge<Edge, std::optional<T>>(node, "elem"); // Edge<std::optional<T>>&
                                                                                                                                                                        auto& elemOpt  = elemEdge.get();                                      // std::optional<T>&
                                                                                                                                                                        fif(
                                                                                                                                                                            [&] { return elemOpt.has_value(); },
                                                                                                                                                                            [&] {
                                                                                                                                                                                elem = elemOpt.value();
                                                                                                                                                                                return true;
                                                                                                                                                                            },
                                                                                                                                                                            [&] { return false; }
                                                                                                                                                                        );
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
                                                                                                                                                                                        
                                                                                                                                                                                        size_t total_matched = elem.text.size() + match.length(0);
                                                                                                                                                                                        consume_chars(total_matched);

                                                                                                                                                                                        matched_identifier = true;
                                                                                                                                                                                        return matched_identifier;
                                                                                                                                                                                    },
                                                                                                                                                                                    [&] {return false;}
                                                                                                                                                                                );
                                                                                                                                                                            },
                                                                                                                                                                            [&] { return false; }
                                                                                                                                                                        );
                                                                                                                                                                    }
                                                                                                                                                                );

                                                                                                                                                                return fif(
                                                                                                                                                                    [&] { return matched_identifier; },
                                                                                                                                                                    [&] { return true; },
                                                                                                                                                                    [&] {
                                                                                                                                                                        return fif(
                                                                                                                                                                            // capture commas
                                                                                                                                                                            [&] { return input[0] == ','; },
                                                                                                                                                                            [&] {
                                                                                                                                                                                return fif(
                                                                                                                                                                                    [&] { return true; },
                                                                                                                                                                                    [&] { 
                                                                                                                                                                                        add_token(LexerToken(std::string(1, static_cast<char>(*consume())), LexerTypes::COMMA(), current_line));
                                                                                                                                                                                        return true; 
                                                                                                                                                                                    },
                                                                                                                                                                                    [&] { return false; }
                                                                                                                                                                                );
                                                                                                                                                                            },
                                                                                                                                                                            [&] {
                                                                                                                                                                                return fif(
                                                                                                                                                                                    [&] { return input[0] == '['; },
                                                                                                                                                                                    [&] {
                                                                                                                                                                                        return fif(
                                                                                                                                                                                            [&] { return true; },
                                                                                                                                                                                            [&] {
                                                                                                                                                                                                add_token(LexerToken(std::string(1, static_cast<char>(*consume())), LexerTypes::ARRAY_START(), current_line));
                                                                                                                                                                                                return true;
                                                                                                                                                                                            },
                                                                                                                                                                                            [&] { return false; }
                                                                                                                                                                                        );
                                                                                                                                                                                    },
                                                                                                                                                                                    [&] {
                                                                                                                                                                                        return fif(
                                                                                                                                                                                            [&] { return input[0] == ']'; },
                                                                                                                                                                                            [&] {
                                                                                                                                                                                                return fif(
                                                                                                                                                                                                    [&] { return true; },
                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                        add_token(LexerToken(std::string(1, static_cast<char>(*consume())), LexerTypes::ARRAY_END(), current_line));
                                                                                                                                                                                                        return true;
                                                                                                                                                                                                    },
                                                                                                                                                                                                    [&] { return false; }
                                                                                                                                                                                                );
                                                                                                                                                                                            },
                                                                                                                                                                                            [&] {
                                                                                                                                                                                                return fif(
                                                                                                                                                                                                    [&] { return input[0] == '{'; },
                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                            [&] { return true; },
                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                add_token(LexerToken(std::string(1, static_cast<char>(*consume())), LexerTypes::OBJECT_START(), current_line));
                                                                                                                                                                                                                return true;
                                                                                                                                                                                                            },
                                                                                                                                                                                                            [&] { return false; }
                                                                                                                                                                                                        );
                                                                                                                                                                                                    },
                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                            [&] { return input[0] == '}'; },
                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                return fif(
                                                                                                                                                                                                                    [&] { return true; },
                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                        add_token(LexerToken(std::string(1, static_cast<char>(*consume())), LexerTypes::OBJECT_END(), current_line));
                                                                                                                                                                                                                        return true;
                                                                                                                                                                                                                    },
                                                                                                                                                                                                                    [&] { return false; }
                                                                                                                                                                                                                );
                                                                                                                                                                                                            },
                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                return fif(
                                                                                                                                                                                                                    [&] { return input[0] == '('; },
                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                                            [&] { return true; },
                                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                                add_token(LexerToken(std::string(1, static_cast<char>(*consume())), LexerTypes::TUPLE_START(), current_line));
                                                                                                                                                                                                                                return true;
                                                                                                                                                                                                                            },
                                                                                                                                                                                                                            [&] { return false; }
                                                                                                                                                                                                                        );
                                                                                                                                                                                                                    },
                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                                            [&] { return input[0] == ')'; },
                                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                                return fif(
                                                                                                                                                                                                                                    [&] { return true; },
                                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                                        add_token(LexerToken(std::string(1, static_cast<char>(*consume())), LexerTypes::TUPLE_END(), current_line));
                                                                                                                                                                                                                                        return true;
                                                                                                                                                                                                                                    },
                                                                                                                                                                                                                                    [&] { return false; }
                                                                                                                                                                                                                                );
                                                                                                                                                                                                                            },
                                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                                return fif(
                                                                                                                                                                                                                                    [&] { return input[0] == '@'; },
                                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                                        return fif(
                                                                                                                                                                                                                                            [&] { return true; },
                                                                                                                                                                                                                                            [&] {
                                                                                                                                                                                                                                                add_token(LexerToken(std::string(1, static_cast<char>(*consume())), LexerTypes::REF(), current_line));
                                                                                                                                                                                                                                                return true;
                                                                                                                                                                                                                                            },
                                                                                                                                                                                                                                            [&] { return false; }
                                                                                                                                                                                                                                        );
                                                                                                                                                                                                                                    },
                                                                                                                                                                                                                                    [&] {
                                                                                                                                                                                                                                        // capture standard unexpected
                                                                                                                                                                                                                                        add_token(LexerToken(split(input, ' ')[0], LexerTypes::UNEXP(), current_line));
                                                                                                                                                                                                                                        return false;
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
                return false;
            }
        );

        // End-of-file token
        add_token(LexerToken("ENDF", LexerTypes::ENDF(), current_line));
        return std::nullopt;
    }
};
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
#endif // LEXER_FUNCTIONAL_FULL_H
