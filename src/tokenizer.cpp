#include "tokenizer.hpp"
#include "file_manager.hpp"
#include "token.hpp"

const char* Tokenizer::OUTPUT_FILENAME
    = "tmp/tokenizer.log";

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

    output();

    tokens.swap(mTokens);
    return mIsValid;
}

void Tokenizer::tokenize()
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

bool Tokenizer::isOperator(Token::EKind& kind)
{
    kind = Token::INTEGRAL;

    if(mSource.at(mIndex) == '+')
        kind = Token::PLUS;
    else if(mSource.at(mIndex) == '-')
        kind = Token::MINUS;
    else if(mSource.at(mIndex) == '*')
        kind = Token::ASTERISK;
    else if(mSource.at(mIndex) == '/')
        kind = Token::VIRGULE;
    else if(mSource.at(mIndex) == '%')
        kind = Token::PERCENT;
    else
        return false;
    
    mIndex++;
    return true;
}

bool Tokenizer::isOther(Token::EKind& kind)
{
    kind = Token::INTEGRAL;

    if(mSource.at(mIndex) == '(')
        kind = Token::OPEN_BRACKET;
    else if(mSource.at(mIndex) == ')')
        kind = Token::CLOSE_BRACKET;
    else if(mSource.at(mIndex) == ';')
        kind = Token::END;
    else
        return false;

    mIndex++;
    return true;
}

bool Tokenizer::isIgnore()
{
    if(mSource.at(mIndex) != ' '  &&
       mSource.at(mIndex) != '\t' &&
       mSource.at(mIndex) != '\n')
        return false;

    mIndex++;
    return true;
}

void Tokenizer::output() const
{
    std::string data("{\n");

    for(std::size_t i = 0; i < mTokens.size(); i++)
    {
        data += "    " + std::to_string(i);
        data += std::string(": \"") + Token::KIND_NAME_MAP.at(mTokens.at(i)->kind);
        data += "\"";

        if(i != mTokens.size() - 1)
            data += ",";
        data += "\n";
    }

    data += "}\n";

    FileManager::write(OUTPUT_FILENAME,
                       data.c_str());
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

bool Tokenizer::error()
{
    std::cerr << "toke-err: unsupported characters "
              << "( " << mSource.at(mIndex) << " )." << std::endl;
    
    mIsValid = false;
    return mIsValid;
}