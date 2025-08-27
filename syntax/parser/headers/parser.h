#ifndef PARSER_H
#define PARSER_H

#include "../../tokens/headers/lexer_token.h"
#include "../../tokens/headers/parser_token.h"

struct Parser {
    std::vector<LexerToken> input;
    std::vector<ParserToken> token_output;

    /*

        this is meant to encode with objects the description of rdata relationships, and operational relationships.
        DO NOT:     
            - attempt to encode recursion, only return objects
            - attempt to encode graph collapse, only the fact that the graph exists, and maybe a null prefix
            to denote the beginning of a collapsable scope
            
    
    */
}

#endif // PARSER_H