#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../../syntax/lexer/headers/lexer.h"
#include "../../syntax/tokens/headers/lexer_token.h" // make sure path is correct

// Function to read a .fx file
std::string read_fx_file(const std::string& relative_path) {
    std::ifstream file(relative_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << relative_path << std::endl;
        return "";
    }

    std::ostringstream buffer;
    buffer << file.rdbuf(); // Read entire file into buffer
    return buffer.str();
}

void print_tokens(const std::vector<LexerToken>& tokens) {
    std::cout << "Tokenization results:\n\n";
    for (const auto& token : tokens) {
        std::cout << "Token(text=\"" << token.value 
                  << "\", type=\"" << token.type << "\")\n";
    }
}


int main() {
    std::string path = "test/test.fx"; // relative path to the .fx file
    std::string content = read_fx_file(path);
    std::vector<char> input_vec(content.begin(), content.end());
    if (!content.empty()) {
        Lexer<char> lexer;

        lexer.set_input(input_vec);
        lexer.tokenize();
        print_tokens(lexer.token_output);
    }

    return 0;
}
