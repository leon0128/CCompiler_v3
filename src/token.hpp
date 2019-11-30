#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class Token
{
public:
    // トークンの種類を表す列挙体
    enum EKind
    {
        // parent
        PARENT, 
        // function, variable
        FUNCTION, VARIABLE,
        // declaration
        DEC_LONG,
        // integral
        INTEGRAL,
        // =, !
        EQUAL, NOT,
        // +, -, *, /, %
        PLUS, MINUS, ASTERISK, VIRGULE, PERCENT,
        // ==, !=, <, <=, >, >=
        CMP_EQUAL, CMP_NOT_EQUAL,
        CMP_LESS, CMP_LESS_EQUAL,
        CMP_GREATER, CMP_GREATER_EQUAL,
        // (, )
        OPEN_BRACKET, CLOSE_BRACKET,
        // ,, ;
        COMMA, END
    };
    // 型
    enum EType
    {
        LONG
    };

    // 特殊関数
    Token(EKind inKind);
    virtual ~Token(){}    
    EKind kind; // トークンの種類    

    // 型の判別
    bool isParent() const;
    bool isFunction() const;
    bool isDeclaration() const;
    bool isOperator() const;
    bool isArithmeticOperator() const;
    bool isAssignmentOperator() const;
    bool isCompareOperator() const;
    bool isVariable() const;
    bool isIntegral() const;
    bool isOther() const;
    
    // dynamic_cast をラップ
    template<typename T>
    static T cast(Token* token){return dynamic_cast<T>(token);}
    // トークンの開放
    static void destroy();

    // トークンの種類と文字列のマップ
    static const std::unordered_map<EKind, const char*> KIND_NAME_MAP;
    // 型の大きさ
    static const std::unordered_map<EType, long> TYPE_SIZE_MAP;
private:
    // 作成したトークンを格納
    static std::vector<Token*> TOKENS;
};

class ParentToken : public Token
{
public:
    ParentToken():
        Token(PARENT),
        children(){}

    std::vector<Token*> children;
};

class FunctionToken : public Token
{
public:
    FunctionToken(const std::string& inName):
        Token(FUNCTION),
        name(inName),
        proc(nullptr),
        offset(0){}

    std::string name;
    Token* proc;
    long offset;
};

class OperatorToken : public Token
{
public:
    OperatorToken(EKind inKind):
        Token(inKind),
        lhs(nullptr),
        rhs(nullptr){}

    Token* lhs;
    Token* rhs;
};

class VariableToken : public Token
{
public:
    VariableToken(const std::string& inName):
        Token(VARIABLE),
        type(LONG),
        name(inName),
        offset(0){}

    EType type;
    std::string name;
    long offset;
};

class IntegralToken : public Token
{
public:
    IntegralToken(long inValue):
        Token(INTEGRAL),
        value(inValue){}

    long value;
};