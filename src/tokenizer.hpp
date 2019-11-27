#pragma once

#include "token.hpp"
#include <vector>
#include <string>

class Tokenizer
{
public:
    Tokenizer();
    ~Tokenizer();

    bool tokenize(std::string& source,
                  std::vector<class Token*> tokens);

private:
    void consume();

    bool isIntegral(long& value);
    bool isOperator(Token::EKind& kind);

    std::string mSource;
    std::vector<class Token*> mTokens;
    std::size_t mIndex;
    bool mIsValid;
};