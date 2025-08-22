#ifndef LEXERTYPES_H
#define LEXERTYPES_H

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

    std::string STRING() {
        return "STRING";
    }

    std::string INTEGER() {
        return "INTEGER";
    }

    std::string FLOAT() {
        return "FLOAT";
    }

    std::string ARRAY() {
        return "ARRAY";
    }

    std::string TUPLE() {
        return "TUPLE";
    }

    std::string MAP() {
        return "MAP";
    }

    std::string FUNC() {
        return "FUNC";
    }

    std::string ANON_FUNC() {
        return "ANON_FUNC";
    }

    std::string KEYWORD() {
        return "KEYWORD";
    }

    std::string DECL() {
        return "DECL";
    }

    std::string SELECTOR() {
        return "SELECTOR";
    }

    std::string OPERATOR() {
        return "OPERATOR";
    }

    std::string METHOD() {
        return "METHOD";
    }

    std::string DECL_METHOD() {
        return "DECL_METHOD";
    }
};

#endif // LEXERTYPES_H