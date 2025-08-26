#ifndef PARSERTOKEN_H
#define PARSERTOKEN_H

    struct ParserToken {
        std::string value;
        std::string type;

        inline ParserToken(const std::string& _value, const std::string& _type)
            : value(_value), type(_type) {}
        
        inline std::string get_value() { return value; }
        inline std::string get_type() { return type; }
    }

#endif // PARSERTOKEN_H