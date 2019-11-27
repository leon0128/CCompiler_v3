#include "tokenizer.hpp"
#include "token.hpp"

Tokenizer::Tokenizer():
    mSource(),
    mTokens(),
    mIndex(0),
    mIsValid(true)
{
}

Tokenizer::~Tokenizer()
{
}

bool Tokenizer::tokenize(std::string& source,
                         std::vector<Token*> tokens)
{
    mSource.swap(source);

    consume();

    return mIsValid;
}

void Tokenizer::consume()
{
    long integral = 0;
    Token::EKind kind = Token::INTEGRAL;

    while(mIndex < mSource.size() && mIsValid)
    {
        if(isIntegral(integral))
            mTokens.emplace_back(new IntegralToken(integral));
        else if(isOperator(kind))
            mTokens.emplace_back(new OperatorToken(kind));
        else if(isOther(kind))
            mTokens.emplace_back(new Token(kind));
        else if(isIgnore())
            ;
        else
            error();
    }
}

bool Tokenizer::isIntegral(long& value)
{
    if(!isNumber())
        return false;

    value = 0;
    while(1)
    {
        if(isNumber())
            value += value * 10 +
                     mSource.at(mIndex++) - '0';
        else
            break;
    }

    return true;
}

bool Tokenizer::isNumber() const
{
    if(mIndex >= mSource.size())
        return false;

    bool isValid
        = (mSource.at(mIndex) >= '0' &&
           mSource.at(mIndex) <= '9')
            ? true : false;

    return isValid;
}