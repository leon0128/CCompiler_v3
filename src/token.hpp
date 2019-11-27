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


    // dynamic_cast をラップ
    template<typename T>
    static T cast(Token* token){return dynamic_cast<T>(token);}
    // トークンの開放
    static void destroy();

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

class IntegralToken : public Token
{
public:
    IntegralToken(long inValue):
        Token(INTEGRAL),
        value(inValue){}

    long value;
};