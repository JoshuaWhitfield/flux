#include <iostream>
#include <vector>
#include <string>

#include "../util/arrays.h" // slice
#include "./headers/lexer_token.h"

template<typename T>

struct Lexer {
    std::vector<std::string> input;
    std::vector<LexerToken> token_output;

    void add_token(LexerToken token) {
        token_output.push(token);
    }

    T get_current() {
        if !sizeof(input) {return false}
        return input[0]  
    }
    
    T consume() {
        std::string current = get_current();
        if !current {return false}
        std::string consumed = current;
        input = slice(input, 1);
    }

};