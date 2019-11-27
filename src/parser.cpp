#include "parser.hpp"
#include "token.hpp"

Parser::Parser():
    mTokens(),
    mParent(nullptr),
    mIndex(0),
    mIsValid(true)
{
}

Parser::~Parser()
{
}

bool Parser::operator()(std::vector<Token*>& tokens,
                        Token*& parent)
{
    mTokens.swap(tokens);

    parse();

    parent = mParent;
    return mIsValid;
}

bool Parser::isValid(Token::EKind kind) const
{
    if(mIndex >= mTokens.size())
        return false;

    if(kind == mTokens.at(mIndex)->kind)
        return true;
    else
        return false;
}

bool Parser::isConsumed(Token::EKind kind)
{
    if(isValid(kind))
    {
        mIndex++;
        return true;
    }
    else
        return false;
}

bool Parser::isErrored(Token::EKind kind)
{
    if(isConsumed(kind))
        return true;
    else
        return error(kind);
}

bool Parser::error(Token::EKind kind)
{
    std::cerr << "pars-err: invalid token "
              << "( ideal: " << Token::KIND_NAME_MAP.at(kind)
              << ", fact: " << Token::KIND_NAME_MAP.at(mTokens.at(mIndex)->kind)
              << " )." << std::endl;

    mIsValid = false;
    return mIsValid;
}