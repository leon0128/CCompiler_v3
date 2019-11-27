#include "token.hpp"

std::vector<Token*> Token::TOKENS
    = std::vector<Token*>();

const std::unordered_map<Token::EKind, const char*> Token::KIND_NAME_MAP
    = {{Token::INTEGRAL, "INTEGRAL"},
       {Token::PLUS, "PLUS"}, {Token::MINUS, "MINUS"}, {Token::ASTERISK, "ASTERISK"}, {Token::VIRGULE, "VIRGULE"}, {Token::PERCENT, "PERCENT"},
       {Token::OPEN_BRACKET, "OPEN_BRACKET"}, {Token::CLOSE_BRACKET, "CLOSE_BRACKET"},
       {Token::END, "END"}};

Token::Token(EKind inKind):
    kind(inKind)
{
    TOKENS.emplace_back(this);
}

void Token::destroy()
{
    for(auto&& token : TOKENS)
        delete token;
}