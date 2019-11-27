#pragma once

#include "token.hpp"
#include <vector>
#include <string>
#include <iostream>

class Tokenizer
{
private:
    static const char* OUTPUT_FILENAME; // 出力ファイル

public:
    Tokenizer();
    ~Tokenizer();

    bool operator()(std::string& source,
                    std::vector<class Token*> tokens);

private:
    void tokenize();

    // consume() で使用する関数
    bool isIntegral(long& value);        // integral
    bool isOperator(Token::EKind& kind); // operator
    bool isOther(Token::EKind& kind);    // other
    bool isIgnore();                     // ignore
    void output() const;                 // debug

    // 数値かどうか
    bool isNumber() const;
    // error 出力
    bool error();

    std::string mSource;
    std::vector<class Token*> mTokens;
    std::size_t mIndex;
    bool mIsValid;
};