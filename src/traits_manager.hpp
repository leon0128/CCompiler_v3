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

    struct VariableTrait
    {
        long scope;
        Token::EType type;
        std::string name;
        long offset;
    };

    void incScope(){mScope++;}
    void decScope();

    bool addVariableTrait(Token* token, Token::EType type);
    bool setVariableTrait(Token* token) const;

    bool error(Token* token, const char* message) const;

    std::vector<VariableTrait> mVariableTraits;
    long mScope;
    std::size_t mOffsetCount;
};