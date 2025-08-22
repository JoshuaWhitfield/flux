#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <cctype>
#include <regex>

#include "../util/arrays.h" // slice
#include "./headers/lexer_token.h"

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
        std::string consumed = current;
        input = slice(input, 1);
        return consumed;
    }

    void set_input(std::vector<std::string> _input) {
        input = _input;
    }

    std::vector<std::string> get_input() {
        return input;
    }

    std::optional<LexerToken> tokenize() {
        while (get_current()) {
            if (std::isspace(get_current())) {
                consume();
                return tokenize();
            }

            if (get_current() == "\\n") {
                add_token(LexerToken("\\n", "ENDL"))
            }
        }
    } 

};