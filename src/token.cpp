#include "token.hpp"

std::vector<Token*> Token::TOKENS
    = std::vector<Token*>();
std::vector<Token::Type*> Token::Type::TYPES
    = std::vector<Token::Type*>();

const std::unordered_map<Token::EKind, const char*> Token::KIND_NAME_MAP
    = {{PARENT, "PARENT"},
       {FUNCTION, "FUNCTION"}, {CALL, "CALL"}, {VARIABLE, "VARIABLE"},
       {WHILE, "WHILE"}, {FOR, "FOR"}, {IF, "IF"}, {ELSE, "ELSE"},
       {DEC_LONG, "DEC_LONG"}, {DEC_INT, "DEC_INT"}, {DEC_SHORT, "DEC_SHORT"}, {DEC_CHAR, "DEC_CHAR"},
       {INTEGRAL, "INTEGRAL"},
       {EQUAL, "EQUAL"}, {NOT, "NOT"}, {AMPERSAND, "AMPERSAND"},
       {PLUS, "PLUS"}, {MINUS, "MINUS"}, {ASTERISK, "ASTERISK"}, {VIRGULE, "VIRGULE"}, {PERCENT, "PERCENT"},
       {CMP_EQUAL, "CMP_EQUAL"}, {CMP_NOT_EQUAL, "CMP_NOT_EQUAL"},
       {CMP_LESS, "CMP_LESS"}, {CMP_LESS_EQUAL, "CMP_LESS_EQUAL"},
       {CMP_GREATER, "CMP_GREATER"}, {CMP_GREATER_EQUAL, "CMP_GREATER_EQUAL"},
       {RETURN, "RETURN"},
       {OPEN_BRACKET, "OPEN_BRACKET"}, {CLOSE_BRACKET, "CLOSE_BRACKET"}, {OPEN_BLOCK, "OPEN_BLOCK"}, {CLOSE_BLOCK, "CLOSE_BLOCK"},
       {COMMA, "COMMA"}, {END, "END"}};

const std::unordered_map<Token::EType, const char*> Token::TYPE_NAME_MAP
    = {{POINTER, "*"}, {LONG, "long"}, {INT, "int"}, {SHORT, "short"}, {CHAR, "char"}};

const std::unordered_map<Token::EType, long> Token::TYPE_SIZE_MAP
    = {{POINTER, 8}, {LONG, 8}, {INT, 4}, {SHORT, 2}, {CHAR, 1}};

const std::unordered_map<Token::EKind, Token::EType> Token::TYPE_DEC_MAP
    = {{DEC_LONG, LONG}, {DEC_INT, INT}, {DEC_SHORT, SHORT}, {DEC_CHAR, CHAR}};

long WhileToken::LABEL_INDEX = 0;
long ForToken::LABEL_INDEX   = 0;
long IfToken::LABEL_INDEX    = 0;

Token::Token(EKind inKind):
    kind(inKind)
{
    TOKENS.emplace_back(this);
}

Token::Type::Type(EType inType):
    type(inType),
    node(nullptr)
{
    TYPES.emplace_back(this);
}

void Token::Type::destroy()
{
    for(auto&& e : TYPES)
        delete e;
}

bool Token::isParent() const
{
    return kind == PARENT;
}

bool Token::isFunction() const
{
    return kind == FUNCTION;
}

bool Token::isCall() const
{
    return kind == CALL;
}

bool Token::isWhile() const
{
    return kind == WHILE;
}

bool Token::isFor() const
{
    return kind == FOR;
}

bool Token::isIf() const
{
    return kind == IF;
}

bool Token::isDeclaration() const
{
    if(kind == DEC_LONG  ||
       kind == DEC_INT   ||
       kind == DEC_SHORT ||
       kind == DEC_CHAR)
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

bool Token::isReturn() const
{
    return kind == RETURN;
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