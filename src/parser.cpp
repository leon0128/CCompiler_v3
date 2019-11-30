#include "parser.hpp"
#include "debugger.hpp"
#include "token.hpp"

Parser::Parser():
    mVariableTraits(),
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
    mParent = unit();
}

Token* Parser::unit()
{
    Token* token = function();

    return token;
}

Token* Parser::function()
{
    FunctionToken* funTok
        = new FunctionToken("main");
    
    ParentToken* parTok
        = new ParentToken();
    
    while(mIndex < mTokens.size() && mIsValid)
        parTok->children.push_back(statement());
    
    funTok->proc = parTok;
    funTok->offset = mVariableTraits.size() * 8;
    
    return funTok;
}

Token* Parser::statement()
{
    Token* token = declaration();
    
    if(!isErrored(Token::END))
        token = nullptr;

    return token;
}

Token* Parser::declaration()
{
    ParentToken* parTok = new ParentToken();

    if(isConsumed(Token::DEC_LONG))
    {
        while(1)
        {
            if(!isErrored(Token::VARIABLE))
                break;
            
            addVariableTrait(mTokens.at(--mIndex), Token::LONG);

            Token* token = expression();

            if(token->kind != Token::VARIABLE &&
               token->kind != Token::EQUAL)
            {
                error("invalid variable declaration.");
                break;
            }

            parTok->children.push_back(token);
            if(!isConsumed(Token::COMMA))
                break;
        }
    }

    return parTok;
}

Token* Parser::expression()
{
    Token* token = assignment();

    return token;
}

Token* Parser::assignment()
{
    Token* token = equality();

    if(isValid(Token::EQUAL))
    {
        if(token->kind != Token::VARIABLE)
            error("invalid lvalue in assignment expression.");
        
        OperatorToken* opeTok
            = Token::cast<OperatorToken*>(mTokens.at(mIndex++));
        
        opeTok->lhs = token;
        opeTok->rhs = assignment();

        token = opeTok;
    }

    return token;
}

Token* Parser::equality()
{
    Token* token = comparison();

    while(1)
    {
        if(isValid(Token::CMP_EQUAL) ||
           isValid(Token::CMP_NOT_EQUAL))
        {
            OperatorToken* opeTok
                = Token::cast<OperatorToken*>(mTokens.at(mIndex++));
            
            opeTok->lhs = comparison();
            opeTok->rhs = comparison();

            token = opeTok;
        }
        else
            break;
    }

    return token;
}

Token* Parser::comparison()
{
    Token* token = addition();

    while(1)
    {
        if(isValid(Token::CMP_LESS)       ||
           isValid(Token::CMP_LESS_EQUAL) ||
           isValid(Token::CMP_GREATER)    ||
           isValid(Token::CMP_GREATER_EQUAL))
        {
            OperatorToken* opeTok
                = Token::cast<OperatorToken*>(mTokens.at(mIndex++));
            
            opeTok->lhs = token;
            opeTok->rhs = addition();

            token = opeTok;
        }
        else
            break;
    }

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
           isValid(Token::VIRGULE)  ||
           isValid(Token::PERCENT))
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

    // 変数
    if(isValid(Token::VARIABLE))
    {
        token = mTokens.at(mIndex++);
        setVariableTrait(token);
    }
    // 整数値
    else if(isValid(Token::INTEGRAL))
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

bool Parser::error(const char* message)
{
    std::cerr << "pars-err: "
              << message
              << std::endl;
    mIsValid = false;
    return mIsValid; 
}

void Parser::addVariableTrait(Token* token, Token::EType type)
{
    VariableToken* varTok
        = Token::cast<VariableToken*>(token);

    for(auto&& e : mVariableTraits)
    {
        if(varTok->name == e.name)
        {
            error("variable opverloading.");
            return;
        }
    }

    varTok->offset
        = (mVariableTraits.size() + 1) * 8;
    mVariableTraits.push_back(VariableTrait{varTok->type,
                                            varTok->name,
                                            varTok->offset});
}

void Parser::setVariableTrait(Token* token)
{
    VariableToken* varTok
        = Token::cast<VariableToken*>(token);

    for(auto&& e : mVariableTraits)
    {
        if(varTok->name == e.name)
        {
            varTok->type =  e.type;
            varTok->offset = e.offset;
            return;
        }
    }

    error("variable not defined.");
}