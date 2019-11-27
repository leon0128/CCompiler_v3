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

    // consume() で使用する関数
    bool isIntegral(long& value);        // integral
    bool isOperator(Token::EKind& kind); // operator
    bool isOther(Token::EKind& kind);    // other
    bool isIgnore();                     // ignore

    // error 出力
    bool error();

    std::string mSource;
    std::vector<class Token*> mTokens;
    std::size_t mIndex;
    bool mIsValid;
};