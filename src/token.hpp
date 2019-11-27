#pragma once

#include <vector>

class Token
{
public:
    enum EKind
    {
        // integral
        INTEGRAL,
        // +, -, *, /, %
        PLUS, MINUS, ASTERISK, VIRGULE, PERCENT,
        // (, )
        OPEN_BRACKET, CLOSE_BRACKET
    };

    // 特殊関数
    Token(EKind inKind);
    virtual ~Token(){}

    EKind kind; // トークンの種類

private:
    static std::vector<Token*> TOKENS; // 作成したトークンを格納
};

class OperatorToken : public Token
{
    OperatorToken(EKind inKind):
        Token(kind),
        lhs(nullptr),
        rhs(nullptr){}
    ~OperatorToken(){}

    Token* lhs; // 左辺値
    Token* rhs; // 右辺値
};