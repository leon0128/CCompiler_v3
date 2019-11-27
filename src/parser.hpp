#pragma once

#include "token.hpp"
#include <vector>
#include <iostream>

class Parser
{
public:
    Parser();
    ~Parser();

    bool operator()(std::vector<class Token*>& tokens,
                    class Token*& parent);

private:
    void parse();

    // parse() で使用する関数
    class Token* statement();      // 文
    class Token* addition();       // 加減
    class Token* multiplication(); // 乗除
    class Token* unary();          // 単項
    class Token* primary();        // 数値、()

    bool isValid(Token::EKind kind) const;
    bool isConsumed(Token::EKind kind);
    bool isErrored(Token::EKind kind);

    bool error(Token::EKind kind);

    std::vector<class Token*> mTokens;
    class Token* mParent;
    std::size_t mIndex;
    bool mIsValid;
};