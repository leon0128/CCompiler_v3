#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"

class TraitsManager
{
private:
    friend class Parser;

    TraitsManager();
    ~TraitsManager();

    struct Trait
    {
        long scope;
        Token::EType type;
        std::string name;
        long offset;
    };

    void incScope(){mScope++;}
    void decScope();

    bool addTrait(Token* token, Token::EType type);
    bool setTrait(Token* token) const;

    bool error(Token* token, const char* message) const;

    std::vector<Trait> mTraits;
    long mScope;
    std::size_t mOffsetCount;
};