#include "tokenizer.hpp"
#include "debugger.hpp"
#include "token.hpp"
#include "static_data.hpp"

const std::unordered_map<std::string, Token::EKind> Tokenizer::RESERVED_WORD_MAP
    = {{"long", Token::DEC_LONG}, {"int", Token::DEC_INT}, {"short", Token::DEC_SHORT}, {"char", Token::DEC_CHAR},
       {"return", Token::RETURN}, {"while", Token::WHILE}, {"for", Token::FOR}, {"if", Token::IF}, {"else", Token::ELSE}};

Tokenizer::Tokenizer():
    mIndex(0),
    mIsValid(true)
{
}

Tokenizer::~Tokenizer()
{
}

bool Tokenizer::operator()()
{
    tokenize();
    
    Debugger::tokenizer(DATA::TOKENIZER_DATA());

    return mIsValid;
}

void Tokenizer::tokenize()
{
    std::string name;
    long integral = 0;
    Token::EKind kind = Token::INTEGRAL;

    while(mIndex < DATA::PREPROCESSOR_DATA().size() && mIsValid)
    {
        if(isString(name, kind))
        {
            if(kind == Token::VARIABLE)
                DATA::TOKENIZER_DATA().emplace_back(new VariableToken(name));
            else if(kind == Token::RETURN)
                DATA::TOKENIZER_DATA().emplace_back(new ReturnToken());
            else if(kind == Token::WHILE)
                DATA::TOKENIZER_DATA().emplace_back(new WhileToken());
            else if(kind == Token::FOR)
                DATA::TOKENIZER_DATA().emplace_back(new ForToken());
            else if(kind == Token::IF)
                DATA::TOKENIZER_DATA().emplace_back(new IfToken());
            else
                DATA::TOKENIZER_DATA().emplace_back(new Token(kind));
        }
        else if(isIntegral(integral))
            DATA::TOKENIZER_DATA().emplace_back(new IntegralToken(integral));
        else if(isOperator(kind))
            DATA::TOKENIZER_DATA().emplace_back(new OperatorToken(kind));
        else if(isOther(kind))
            DATA::TOKENIZER_DATA().emplace_back(new Token(kind));
        else if(isIgnore())
            ;
        else
            error();
    }
}

bool Tokenizer::isString(std::string& name, Token::EKind& kind)
{
    if(!isAlphabet())
        return false;

    name.clear();
    kind = Token::VARIABLE;
    while(1)
    {
        if(isAlphabet() ||
           isNumber())
            name.push_back(DATA::PREPROCESSOR_DATA().at(mIndex++));
        else
            break;
    }

    for(auto&& pair : RESERVED_WORD_MAP)
    {
        if(pair.first == name)
        {
            kind = pair.second;
            break;
        }
    }

    return true;
}

bool Tokenizer::isIntegral(long& value)
{
    if(!isNumber())
        return false;

    value = 0;
    while(1)
    {
        if(isNumber())
            value = value * 10 +
                 DATA::PREPROCESSOR_DATA().at(mIndex++) - '0';
        else
            break;
    }

    return true;
}

bool Tokenizer::isOperator(Token::EKind& kind)
{
    kind = Token::INTEGRAL;
    
    if(isValid('+'))
        kind = Token::PLUS;
    else if(isValid('-'))
        kind = Token::MINUS;
    else if(isValid('*'))
        kind = Token::ASTERISK;
    else if(isValid('/'))
        kind = Token::VIRGULE;
    else if(isValid('%'))
        kind = Token::PERCENT;
    else if(isValid('='))
    {
        mIndex++;
        if(isValid('='))
            kind = Token::CMP_EQUAL;
        else
            kind = Token::EQUAL;
    }
    else if(isValid('!'))
    {
        mIndex++;
        if(isValid('='))
            kind = Token::CMP_NOT_EQUAL;
        else
            kind = Token::NOT;
    }
    else if(isValid('<'))
    {
        mIndex++;
        if(isValid('='))
            kind = Token::CMP_LESS_EQUAL;
        else
            kind = Token::CMP_LESS;
    }
    else if(isValid('>'))
    {
        mIndex++;
        if(isValid('='))
            kind = Token::CMP_GREATER_EQUAL;
        else
            kind = Token::CMP_GREATER;
    }
    else
        return false;

    mIndex++;
    return true;
}

bool Tokenizer::isOther(Token::EKind& kind)
{
    kind = Token::INTEGRAL;

    if(isValid('('))
        kind = Token::OPEN_BRACKET;
    else if(isValid(')'))
        kind = Token::CLOSE_BRACKET;
    else if(isValid('{'))
        kind = Token::OPEN_BLOCK;
    else if(isValid('}'))
        kind = Token::CLOSE_BLOCK;
    else if(isValid(','))
        kind = Token::COMMA;
    else if(isValid(';'))
        kind = Token::END;
    else
        return false;

    mIndex++;
    return true;
}

bool Tokenizer::isIgnore()
{
    if(!isValid(' ')  &&
       !isValid('\t') &&
       !isValid('\n'))
        return false;

    mIndex++;
    return true;
}

bool Tokenizer::isValid(char c) const
{
    if(mIndex >= DATA::PREPROCESSOR_DATA().size())
        return false;
    
    return (c == DATA::PREPROCESSOR_DATA().at(mIndex));
}

bool Tokenizer::isNumber() const
{
    if(mIndex >= DATA::PREPROCESSOR_DATA().size())
        return false;

    bool isValid
        =  (DATA::PREPROCESSOR_DATA().at(mIndex) >= '0' &&
            DATA::PREPROCESSOR_DATA().at(mIndex) <= '9');

    return isValid;
}

bool Tokenizer::isAlphabet() const
{
    if(mIndex >= DATA::PREPROCESSOR_DATA().size())
        return false;
    
    bool isValid
        = (( DATA::PREPROCESSOR_DATA().at(mIndex) >= 'a' && DATA::PREPROCESSOR_DATA().at(mIndex) <= 'z') ||
           (DATA::PREPROCESSOR_DATA().at(mIndex) >= 'A' && DATA::PREPROCESSOR_DATA().at(mIndex) <= 'Z') ||
            DATA::PREPROCESSOR_DATA().at(mIndex) == '_');
    
    return isValid;
}

bool Tokenizer::error()
{
    std::cerr << "toke-err: unsupported characters "
              << "( " << DATA::PREPROCESSOR_DATA().at(mIndex) << " )." << std::endl;
    
    mIsValid = false;
    return mIsValid;
}