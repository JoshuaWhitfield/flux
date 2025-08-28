#ifndef PARSER_H
#define PARSER_H


#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <cctype>
#include <regex>
#include <numeric>

#include "../../../util/headers/debug.h"
#include "../../../util/headers/arrays.h"
#include "../../../util/headers/strings.h"

#include "../../tokens/headers/lexer_token.h"
#include "../../tokens/headers/parser_token.h"
#include "../../types/headers/parser_types.h"

#include "../../../functional/control_flow/ffor.h"
#include "../../../functional/control_flow/fwhile.h"
#include "../../../functional/control_flow/fif.h"
#include "../../../functional/control_flow/bincase.h"
#include "../../../functional/operators/structure/ffltr.h"

struct Parser {
    std::vector<LexerToken> input;
    std::vector<ParserToken> token_output;

    

    /*

        this is meant to encode with objects the description of rdata relationships, and operational relationships.
        DO NOT:     
            - attempt to encode recursion, only return objects
            - attempt to encode graph collapse, only the fact that the graph exists, and maybe a null prefix
            to denote the beginning of a collapsable scope
            - do not attempt to make paser objects self evaluating
        DO: 
            - add metadata to each parser object for introspection
            - generate children tokens in vector of LexerTokens before producing the ParserToken
    
    */
}

#endif // PARSER_H