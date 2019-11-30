#pragma once

#include "token.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class Tokenizer
{
private:
    static const std::unordered_map<std::string, Token::EKind> RESERVED_WORD_MAP;

public:
    Tokenizer();
    ~Tokenizer();

    bool operator()(std::string& source,
                    std::vector<class Token*>& tokens);

private:
    void tokenize();

    // consume() で使用する関数
    bool isString(std::string& name, Token::EKind& kind); // string
    bool isIntegral(long& value);        // integral
    bool isOperator(Token::EKind& kind); // operator
    bool isOther(Token::EKind& kind);    // other
    bool isIgnore();                     // ignore

    bool isValid(char c) const; // 指定した文字
    bool isNumber() const;      // 数値
    bool isAlphabet() const;    // アルファベット又はアンダーバー 
    // error 出力
    bool error();

    std::string mSource;
    std::vector<class Token*> mTokens;
    std::size_t mIndex;
    bool mIsValid;
};