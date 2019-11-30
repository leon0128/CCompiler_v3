#include "tokenizer.hpp"
#include "debugger.hpp"
#include "token.hpp"

const std::unordered_map<std::string, Token::EKind> Tokenizer::RESERVED_WORD_MAP
    = {{"long", Token::DEC_LONG}};

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

bool Tokenizer::operator()(std::string& source,
                           std::vector<Token*>& tokens)
{
    mSource.swap(source);

    tokenize();

    Debugger::tokenizer(mTokens);

    tokens.swap(mTokens);
    return mIsValid;
}

void Tokenizer::tokenize()
{
    std::string name;
    long integral = 0;
    Token::EKind kind = Token::INTEGRAL;

    while(mIndex < mSource.size() && mIsValid)
    {
        if(isString(name, kind))
        {
            if(kind == Token::VARIABLE)
                mTokens.emplace_back(new VariableToken(name));
            else
                mTokens.emplace_back(new Token(kind));
        }
        else if(isIntegral(integral))
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
            name.push_back(mSource.at(mIndex++));
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
                    mSource.at(mIndex++) - '0';
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
    if(mIndex >= mSource.size())
        return false;
    
    return (c == mSource.at(mIndex));
}

bool Tokenizer::isNumber() const
{
    if(mIndex >= mSource.size())
        return false;

    bool isValid
        = (mSource.at(mIndex) >= '0' &&
           mSource.at(mIndex) <= '9');

    return isValid;
}

bool Tokenizer::isAlphabet() const
{
    if(mIndex >= mSource.size())
        return false;
    
    bool isValid
        = ((mSource.at(mIndex) >= 'a' && mSource.at(mIndex) <= 'z') ||
           (mSource.at(mIndex) >= 'A' && mSource.at(mIndex) <= 'Z') ||
           mSource.at(mIndex) == '_');
    
    return isValid;
}

bool Tokenizer::error()
{
    std::cerr << "toke-err: unsupported characters "
              << "( " << mSource.at(mIndex) << " )." << std::endl;
    
    mIsValid = false;
    return mIsValid;
}