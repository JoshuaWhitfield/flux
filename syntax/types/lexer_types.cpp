#include <iostream>
#include <string>

struct LexerTypes {
    std::string ENDL() {
        return "ENDL";
    }

    std::string ENDF() {
        return "EOF";
    }

    std::string CONST() {
        return "CONST";
    }

    std::string LET() {
        return "let";
        /*
            #include <iostream>
            #include <string>
            #include <cctype> // for std::isspace

            // Function to parse value after a keyword
            std::string parse_after_keyword(const std::string& text, const std::string& keyword) {
                size_t pos = text.find(keyword);
                if (pos == std::string::npos) return ""; // keyword not found

                pos += keyword.length(); // move past the keyword

                // skip whitespace after the keyword
                while (pos < text.length() && std::isspace(text[pos])) pos++;

                // extract the rest of the string as the value
                return text.substr(pos);
            }

        */
    }

    std::string

    std::string

    std::string

    std::string

    std::string
};