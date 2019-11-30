#include "token.hpp"

std::vector<Token*> Token::TOKENS
    = std::vector<Token*>();

const std::unordered_map<Token::EKind, const char*> Token::KIND_NAME_MAP
    = {{PARENT, "PARENT"},
       {FUNCTION, "FUNCTION"}, {VARIABLE, "VARIABLE"},
       {DEC_LONG, "DEC_LONG"},
       {INTEGRAL, "INTEGRAL"},
       {EQUAL, "EQUAL"}, {NOT, "NOT"},
       {PLUS, "PLUS"}, {MINUS, "MINUS"}, {ASTERISK, "ASTERISK"}, {VIRGULE, "VIRGULE"}, {PERCENT, "PERCENT"},
       {CMP_EQUAL, "CMP_EQUAL"}, {CMP_NOT_EQUAL, "CMP_NOT_EQUAL"},
       {CMP_LESS, "CMP_LESS"}, {CMP_LESS_EQUAL, "CMP_LESS_EQUAL"},
       {CMP_GREATER, "CMP_GREATER"}, {CMP_GREATER_EQUAL, "CMP_GREATER_EQUAL"},
       {OPEN_BRACKET, "OPEN_BRACKET"}, {CLOSE_BRACKET, "CLOSE_BRACKET"},
       {COMMA, "COMMA"}, {END, "END"}};

const std::unordered_map<Token::EType, long> Token::TYPE_SIZE_MAP
    = {{LONG, 8}};

Token::Token(EKind inKind):
    kind(inKind)
{
    TOKENS.emplace_back(this);
}

bool Token::isParent() const
{
    return kind == PARENT;
}

bool Token::isFunction() const
{
    return kind == FUNCTION;
}

bool Token::isDeclaration() const
{
    if(kind == DEC_LONG)
        return true;
    else
        return false;
}

bool Token::isOperator() const
{
    if(isArithmeticOperator() ||
       isAssignmentOperator() ||
       isCompareOperator())
        return true;
    else
        return false;
}

bool Token::isArithmeticOperator() const
{
    if(kind == PLUS     ||
       kind == MINUS    ||
       kind == ASTERISK ||
       kind == VIRGULE  ||
       kind == PERCENT)
        return true;
    else
        return false;
}

bool Token::isAssignmentOperator() const
{
    if(kind == EQUAL)
        return true;
    else
        return false;
}

bool Token::isCompareOperator() const
{
    if(kind == CMP_EQUAL      ||
       kind == CMP_NOT_EQUAL  ||
       kind == CMP_LESS       ||
       kind == CMP_LESS_EQUAL ||
       kind == CMP_GREATER    ||
       kind == CMP_GREATER_EQUAL)
        return true;
    else
        return false;
}

bool Token::isVariable() const
{
    return kind == VARIABLE;
}

bool Token::isIntegral() const
{
    return kind == INTEGRAL;
}

bool Token::isOther() const
{
    if(kind == OPEN_BRACKET  ||
       kind == CLOSE_BRACKET ||
       kind == END)
        return true;
    else
        return false;
}

void Token::destroy()
{
    for(auto&& token : TOKENS)
        delete token;
}