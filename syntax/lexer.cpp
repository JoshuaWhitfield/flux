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

/* 

    tokenize() will return a LexerToken value when there is an 
    unexpected value. this will allow us to throw errors down 
    the line.

*/

struct Lexer {
    std::vector<std::string> input;
    std::vector<LexerToken> token_output;

    void add_token(LexerToken token) {
        token_output.push_back(token);
    }

    std::optional<std::string> get_current() {
        if (input.empty()) return std::nullopt;
        return input[0];
    }
    
    std::optional<std::string> consume() {
        std::string current = get_current();
        if (!current) return std::nullopt;
        std::string consumed = current.value();
        input = slice(input, 1);
        return consumed;
    }

    void consume_chars(std::vector<std::string>& input, size_t n) {
        if (n > input.size()) n = input.size();
        input.erase(input.begin(), input.begin() + n);
    }

    void set_input(std::vector<std::string> _input) {
        input = _input;
    }

    std::vector<std::string> get_input() {
        return input;
    }

    std::optional<LexerToken> tokenize() {
        while (get_current()) {
            if (std::isspace(get_current().value())) {
                consume();
                return tokenize();
            }

            if (get_current().value() == "\\n") {
                add_token(LexerToken("\\n", TokenTypes.ENDL()));
                consume();
                return tokenize();
            }
            
            std::string keyword = "const ";
            if (join(input).substr(0, keyword.size()) == keyword) {
                std::smatch match;
                std::regex pattern(R"(\s*([A-Za-z][A-Za-z0-9]*))");
                std::string after_keyword = join(input).substr(keyword.size());

                if (std::regex_search(after_keyword, match, pattern) && match.position() == 0) {
                    std::string identifier = match[1];
                    add_token(TokenTypes(identifier, TokenTypes.CONST()));
                    size_t total_matched = keyword.size() + match.length(0);
                    consume_chars(input, total_matched);
                }    
            }

            std::string keyword = "let ";
            if (join(input).substr(0, keyword.size()) == keyword) {
                std::smatch match;
                std::regex pattern(R"(\s*([A-Za-z][A-Za-z0-9]*))");
                std::string after_keyword = join(input).substr(keyword.size());

                if (std::regex_search(after_keyword, match, pattern) && match.position() == 0) {
                    std::string identifier = match[1];
                    add_token(TokenTypes(identifier, TokenTypes.CONST()));
                    size_t total_matched = keyword.size() + match.length(0);
                    consume_chars(input, total_matched);
                }    
            }

            add_token(LexerToken("", TokenTypes.ENDF()));
            return std::nullopt;
        }
    } 

};