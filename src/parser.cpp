#include "parser.hpp"
#include "debugger.hpp"
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

    Debugger::parser(mParent);

    parent = mParent;
    return mIsValid;
}

void Parser::parse()
{
    mParent = statement();
}

Token* Parser::statement()
{
    Token* token = expression();
    
    if(!isErrored(Token::END))
        token = nullptr;

    return token;
}

Token* Parser::expression()
{
    Token* token = addition();

    return token;
}

Token* Parser::addition()
{
    Token* token = multiplication();
    
    while(1)
    {
        if(isValid(Token::PLUS) ||
           isValid(Token::MINUS))
        {
            OperatorToken* opeTok
                = Token::cast<OperatorToken*>(mTokens.at(mIndex++));

            opeTok->lhs = token;
            opeTok->rhs = multiplication();

            token = opeTok;
        }
        else
            break;
    }

    return token;
}

Token* Parser::multiplication()
{
    Token* token = unary();

    while(1)
    {
        if(isValid(Token::ASTERISK) ||
           isValid(Token::VIRGULE))
        {
            OperatorToken* opeTok
                = Token::cast<OperatorToken*>(mTokens.at(mIndex++));
            
            opeTok->lhs = token;
            opeTok->rhs = unary();

            token = opeTok;
        }
        else
            break;
    }

    return token;
}

Token* Parser::unary()
{
    Token* token = nullptr;

    if(isValid(Token::PLUS) ||
       isValid(Token::MINUS))
    {
        OperatorToken* opeTok
            = Token::cast<OperatorToken*>(mTokens.at(mIndex++));

        opeTok->lhs = new IntegralToken(0);
        opeTok->rhs = primary();

        token = opeTok;
    }
    else
    {
        token = primary();
    }

    return token;
}

Token* Parser::primary()
{
    Token* token = nullptr;

    // 整数値
    if(isValid(Token::INTEGRAL))
    {
        token = mTokens.at(mIndex++);
    }
    // ()
    else if(isConsumed(Token::OPEN_BRACKET))
    {
        token = expression();
        if(!isErrored(Token::CLOSE_BRACKET))
            token = nullptr;
    }
    // error
    else
    {
        isErrored(Token::OPEN_BRACKET);
        token = nullptr;
    }

    return token;
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