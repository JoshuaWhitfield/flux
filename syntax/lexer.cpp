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

            // Skip whitespace
            if (std::isspace(current[0])) {
                consume();
                continue;
            }

            // Handle newline
            if (current == "\\n") {
                add_token(LexerToken("\\n", TokenTypes.ENDL()));
                consume();
                continue;
            }

            // Join remaining input once per iteration
            std::string remaining = join(input);

            // Keyword definitions
            struct Keyword {
                std::string text;
                std::string type;
                Keyword(std::string& _text, std::string& type) 
                    : text(_text), type(_type) {}
            };

            std::vector<Keyword> keywords = {
                Keyword("const ", TokenTypes.CONST()),
                Keyword("let ", TokenTypes.LET())
            };

            bool matchedKeyword = false;
            for (auto& kw : keywords) {
                if (remaining.substr(0, kw.text.size()) != kw.text) {
                    continue; // skip to next keyword
                }

                if (remaining.substr(0, kw.text.size()) == kw.text) {
                    std::string after_keyword = remaining.substr(kw.text.size());

                    std::regex pattern(R"(\s*([A-Za-z][A-Za-z0-9]*))");
                    std::smatch match;

                    if (std::regex_search(after_keyword, match, pattern) && match.position() == 0) {
                        std::string identifier = match[1];
                        add_token(TokenTypes(identifier, kw.type));

                        size_t total_matched = kw.text.size() + match.length(0);
                        consume_chars(total_matched);

                        matchedKeyword = true;
                        break; // keyword handled, move to next iteration
                    }
                }
            }

            if (matchedKeyword) continue;

            
        }

        // End-of-file token
        add_token(LexerToken("", TokenTypes.ENDF()));
        return std::nullopt;
    }
};
