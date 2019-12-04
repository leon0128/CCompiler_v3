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

    struct FunctionTrait
    {
        Token::EType type;
        std::string name;
        std::vector<Token::EType> argsType;
    };
    struct VariableTrait
    {
        long scope;
        Token::EType type;
        std::string name;
        long offset;
    };

    void incScope(){mScope++;}
    void decScope();

    bool addFunctionTrait(Token* token);
    bool addVariableTrait(Token* token);
    bool setVariableTrait(Token* token) const;

    bool error(Token* token, const char* message) const;

    std::vector<FunctionTrait> mFunctionTraits;
    std::vector<VariableTrait> mVariableTraits;
    long mScope;
    std::size_t mOffsetCount;
};