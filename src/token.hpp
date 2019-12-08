#pragma once

#include "operand.hpp"
#include <vector>
#include <iostream>
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
        // function, call, variable
        FUNCTION, CALL, VARIABLE,
        // while, for, if, else
        WHILE, FOR, IF, ELSE,
        // declaration
        DEC_LONG, DEC_INT, DEC_SHORT, DEC_CHAR,
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
        // return
        RETURN,
        // (, ), {, }
        OPEN_BRACKET, CLOSE_BRACKET, OPEN_BLOCK, CLOSE_BLOCK,
        // ,, ;
        COMMA, END
    };
    // 型
    enum EType
    {
        POINTER, LONG, INT, SHORT, CHAR
    };
    // 型構造体
    class Type
    {
    private:
        static std::vector<Type*> TYPES;
    public:
        Type(EType inType);

        EType type; // 型
        Type* node; // ポインタの場合のノード

        static void destroy();
    };

    // 特殊関数
    Token(EKind inKind);
    virtual ~Token(){}    
    EKind kind; // トークンの種類    

    // 型の判別
    bool isParent() const;
    bool isFunction() const;
    bool isCall() const;
    bool isWhile() const;
    bool isFor() const;
    bool isIf() const;
    bool isDeclaration() const;
    bool isOperator() const;
    bool isArithmeticOperator() const;
    bool isAssignmentOperator() const;
    bool isCompareOperator() const;
    bool isReturn() const;
    bool isVariable() const;
    bool isIntegral() const;
    bool isOther() const;
    
    // dynamic_cast をラップ
    template<typename T>
    static T cast(Token* token)
    {
        T casTok = dynamic_cast<T>(token);   
        if(!casTok)
            std::cerr << "toke-err: failed to dynamic_cast." << std::endl;
        return casTok;
    }
    // トークンの開放
    static void destroy();

    // トークンの種類と文字列のマップ
    static const std::unordered_map<EKind, const char*> KIND_NAME_MAP;
    // 型名
    static const std::unordered_map<EType, const char*> TYPE_NAME_MAP;
    // 型の大きさ
    static const std::unordered_map<EType, long> TYPE_SIZE_MAP;
    // 宣言に対応する型
    static const std::unordered_map<EKind, EType> TYPE_DEC_MAP;
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
    FunctionToken(const std::string& inName, Type* inType):
        Token(FUNCTION),
        type(inType),
        name(inName),
        argsType(),
        proc(nullptr),
        offset(0){}

    Type* type;
    std::string name;
    std::vector<Type*> argsType;
    Token* proc;
    long offset;
};

class CallToken : public Token
{
public:
    CallToken(std::string& inName):
        Token(CALL),
        type(nullptr),
        name(inName),
        args(){}

    Type* type;
    std::string name;
    std::vector<Token*> args;
};

class WhileToken : public Token
{
private:
    static long LABEL_INDEX;

public:
    WhileToken():
        Token(WHILE),
        cmp(nullptr),
        proc(nullptr),
        label(LABEL_INDEX++){}

    Token* cmp;
    Token* proc;
    long label;
};

class ForToken : public Token
{
private:
    static long LABEL_INDEX;

public:
    ForToken():
        Token(FOR),
        init(nullptr),
        cmp(nullptr),
        rate(nullptr),
        proc(nullptr),
        label(LABEL_INDEX++){}

    Token* init;
    Token* cmp;
    Token* rate;
    Token* proc;
    long label;
};

class IfToken : public Token
{
private:
    static long LABEL_INDEX;

public:
    IfToken():
        Token(IF),
        children(),
        label(LABEL_INDEX++){}

    struct IfChild
    {
        Token* cmp;
        Token* proc;
    };
    std::vector<IfChild> children;
    long label;
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

class ReturnToken : public Token
{
public:
    ReturnToken():
        Token(RETURN),
        expr(nullptr){}

    Token* expr;
};

class VariableToken : public Token
{
public:
    VariableToken(const std::string& inName):
        Token(VARIABLE),
        type(nullptr),
        name(inName),
        offset(0),
        isArg(false){}

    Type* type;
    std::string name;
    long offset; // ローカル変数の場合は rbp からの offset, 仮引数の場合は index
    bool isArg;
};

class IntegralToken : public Token
{
public:
    IntegralToken(long inValue):
        Token(INTEGRAL),
        value(inValue){}

    long value;
};