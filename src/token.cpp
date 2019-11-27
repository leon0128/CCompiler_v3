#include "token.hpp"

std::vector<Token*> Token::TOKENS
    = std::vector<Token*>();

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