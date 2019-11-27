#pragma once

#include <vector>
#include <unordered_map>

class Token
{
public:
    // トークンの種類を表す列挙体
    enum EKind
    {
        // integral
        INTEGRAL,
        // +, -, *, /, %
        PLUS, MINUS, ASTERISK, VIRGULE, PERCENT,
        // (, )
        OPEN_BRACKET, CLOSE_BRACKET,
        // ;
        END
    };

    // 特殊関数
    Token(EKind inKind);
    virtual ~Token(){}    
    EKind kind; // トークンの種類    

    // 型の判別
    bool isIntegral() const;
    bool isOperator() const;
    bool isOther() const;
    
    // dynamic_cast をラップ
    template<typename T>
    static T cast(Token* token){return dynamic_cast<T>(token);}
    // トークンの開放
    static void destroy();

    // トークンの種類と文字列のマップ
    static const std::unordered_map<EKind, const char*> KIND_NAME_MAP;
private:
    // 作成したトークンを格納
    static std::vector<Token*> TOKENS;
};

class OperatorToken : public Token
{
public:
    OperatorToken(EKind inKind):
        Token(inKind),
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