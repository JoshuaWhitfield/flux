#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <cctype>
#include <regex>

#include "../util/arrays.h" // slice join
#include "./tokens/headers/lexer_token.h"
#include "./types/headers/lexer_types.h"

template<typename T>
struct Lexer {
    std::vector<std::string> input;
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
        return consumed;
    }

    void consume_chars(size_t n) {
        if (n > input.size()) n = input.size();
        input.erase(input.begin(), input.begin() + n);
    }

    void set_input(const std::vector<std::string>& _input) {
        input = _input;
    }

    std::vector<std::string> get_input() const {
        return input;
    }

    std::optional<LexerToken> tokenize() {
        while (!input.empty()) {
            auto currentOpt = get_current();
            if (!currentOpt) break;

            std::string current = currentOpt.value();
            if (current[0].empty()) {
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

            // Join remaining input once per iteration
            std::string remaining = remaining;

            // capture identifiers
            struct Identifier {
                std::string text;
                std::string type;
                Identifier(std::string& _text, std::string& type) 
                    : text(_text), type(_type) {}
            };
            
            std::vector<Identifier> identifiers = {
                Identifier("const ", TokenTypes.CONST()),
                Identifier("let ", TokenTypes.LET())
            };
            
            bool matchedKeyword = false;
            for (auto& id : identifiers) {
                if (remaining.substr(0, id.text.size()) != id.text) {
                    continue; // skip to next identifier
                }

                if (remaining.substr(0, id.text.size()) == id.text) {
                    std::string after_identifier = remaining.substr(id.text.size());

                    std::regex pattern(R"(([A-Za-z][A-Za-z0-9]*))");
                    std::smatch match;

                    if (std::regex_search(after_identifier, match, pattern) && match.position() == 0) {
                        std::string identifier = match[1];
                        add_token(TokenTypes(identifier, id.type));

                        size_t total_matched = id.text.size() + match.length(0);
                        consume_chars(total_matched);

                        matchedIdentifier = true;
                        break; // identifier handled, move to next iteration
                    }
                }
            }

            if (matchedIdentifier) continue;

            // capture strings
            if (input[0] == "'" || input[0] == '"') {
                std::smatch match;
                std::regex pattern(R"(\"([A-Za-z0-9]+)\"|\'([A-Za-z0-9]+)\')");
                
                if (std::regex_search(remaining, match, pattern) && match.position() == 0) {
                    std::string str;
                    
                    if (match[1].matched) {
                        str = match[1].str();
                    } else if (match[2].matched) {
                        str = match[2].str();
                    }

                    add_token(LexerToken(str, LexerTypes.STRING()));
                    consume_chars(str.size());
                    continue;
                }
            }

            // capture integers
            if (std::isdigit(static_cast<unsigned char>(input[0]))) {
                std::smatch match; 
                std::regex pattern(R"(\d+)");

                if (std::regex_search(remaining, match, pattern) && match.position() == 0) {
                    int integer = std::stoi(match[0].str()); // typecast from string to integer
                    add_token(LexerToken(integer, LexerTypes.INTEGER()));
                    consume_chars(match.length(0));
                    continue;
                }
            }

            // capture floating points
            if (std::isdigit(static_cast<unsigned char>(input[0])) || input[0] == ".") {
                std::smatch match;
                std::regex pattern(R"(\d*\.\d+)");

                if (std::regex_search(remaining, match, pattern) && match.position() == 0) {
                    float floating_pt = std::stof(match[0].str()); // typecast from string to float
                    add_token(LexerToken(floating_pt, LexerTypes.FLOAT()));
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



            // capture standard unexpected
            add_token(LexerToken("", TokenTypes.UNEXP()));
            break;

            /*
            
                if the Token("UNEXPECTED") contains an empty string as
                a value, then it is a traditional unexpected token. 
                it is handled variably and dynamically depending on 
                where is is in the parsing process.
            
            */

        }

        // capture end-of-file
        add_token(LexerToken("", TokenTypes.ENDF()));
        return std::nullopt;
    }
};
