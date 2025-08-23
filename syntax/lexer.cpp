#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <cctype>
#include <regex>

#include "../util/arrays.h" // slice join
#include "../util/strings.h" // str_slice
#include "./tokens/headers/lexer_token.h"
#include "./types/headers/lexer_types.h"

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

                    std::regex pattern(R"(([A-Za-z][A-Za-z0-9]*))");
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

                        std::regex pattern(R"(([A-Za-z][A-Za-z0-9]*))");
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
                Keyword("return", LexerTypes.KEYWORD()),
                Keyword("case", LexerTypes.KEYWORD()),
                Keyword("concede", LexerTypes.KEYWORD()),
                Keyword("default", LexerTypes.KEYWORD()),
            }

            bool matched_keyword = false;
            for (auto& kw : keywords) {
                if (joined_input.substr(0, kw.text.size()) == kw.text) {
                    // check that next char is a boundary
                    char next_char = joined_input[kw.text.size()];
                    if (std::isspace(next_char) || next_char == '(' || next_char == ';' || nextChar == '\0') {
                        add_token(LexerToken(kw.text, kw.type));
                        consume_chars(kw.text.size());
                        matched_keyword = true;
                        break; // keyword matched
                    }
                }
            }

            if (matched_keyword) continue;

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
                CoreFunction("len", LexerTypes.CORE_FUNC()),
                CoreFunction("type", LexerTypes.CORE_FUNC()),
                CoreFunction("panic", LexerTypes.CORE_FUNC())
            }

            bool matched_core_func = false;
            for (auto& cf : core_functions) {
                if (joined_input.substr(0, cf.text.size()) == cf.text) {
                    // ensure next char is boundary
                    char next_char = joined_input[cf.text.size()];
                    if (std::isspace(next_char) || next_char == "(" || next_char == ";" || next_char == "\0" || next_char == "\n") {
                        add_token(LexerToken(cf.text, cf.type));
                        consume_chars(cf.text.size());
                        matched_core_func = true;
                        break;
                    }
                }
            }

            if (matched_core_func) continue;

            // capture standard unexpected
            add_token(LexerToken("", TokenTypes.UNEXP()));
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
