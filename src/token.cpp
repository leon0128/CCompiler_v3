#include "token.hpp"

std::vector<Token*> Token::TOKENS
    = std::vector<Token*>();

const std::unordered_map<Token::EKind, const char*> Token::KIND_NAME_MAP
    = {{PARENT, "PARENT"},
       {INTEGRAL, "INTEGRAL"},
       {PLUS, "PLUS"}, {MINUS, "MINUS"}, {ASTERISK, "ASTERISK"}, {VIRGULE, "VIRGULE"}, {PERCENT, "PERCENT"},
       {OPEN_BRACKET, "OPEN_BRACKET"}, {CLOSE_BRACKET, "CLOSE_BRACKET"},
       {END, "END"}};

Token::Token(EKind inKind):
    kind(inKind)
{
    TOKENS.emplace_back(this);
}

bool Token::isParent() const
{
    return kind == PARENT;
}

bool Token::isOperator() const
{
    if(kind == PLUS     ||
       kind == MINUS    ||
       kind == ASTERISK ||
       kind == VIRGULE  ||
       kind == PERCENT)
        return true;
    else
        return false;
}

bool Token::isIntegral() const
{
    return kind == INTEGRAL;
}

bool Token::isOther() const
{
    if(kind == OPEN_BRACKET  ||
       kind == CLOSE_BRACKET ||
       kind == END)
        return true;
    else
        return false;
}

void Token::destroy()
{
    for(auto&& token : TOKENS)
        delete token;
}