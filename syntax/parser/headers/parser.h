#ifndef PARSER_H
#define PARSER_H

#include "../../tokens/headers/lexer_token.h"
#include "../../tokens/headers/parser_token.h"

struct Parser {
    std::vector<LexerToken> input;
    std::vector<ParserToken> token_output;

    
}

#endif // PARSER_H