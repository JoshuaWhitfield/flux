#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../../syntax/lexer/headers/lexer.h"
#include "../../syntax/tokens/headers/lexer_token.h"
#include "../../util/headers/debug.h" // print

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

// Write tokens to results file
void write_tokens(const std::vector<LexerToken>& tokens, const std::string& out_path) {
    std::ofstream out(out_path);
    if (!out.is_open()) {
        std::cerr << "Error: Could not open file " << out_path << " for writing\n";
        return;
    }

    out << "Tokenization results:\n\n";
    for (const auto& token : tokens) {
        out << "Token(text=\"" << token.value 
            << "\", type=\"" << token.type << "\")\n";
    }

    out.close();
    std::cout << "Tokens written to " << out_path << std::endl;
}

int main() {
    std::string input_path = "test/test.fx";       // relative path to the .fx file
    std::string output_path = "test/results.txt";  // where tokens will be stored

    std::string content = read_fx_file(input_path);
    std::string input(content.begin(), content.end());

    if (!content.empty()) {
        Lexer<char> lexer;
        lexer.set_input(input);
        lexer.tokenize();

        // Write tokenization results to file
        write_tokens(lexer.token_output, output_path);
    }

    return 0;
}
