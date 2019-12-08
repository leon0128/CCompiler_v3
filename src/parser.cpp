#include "parser.hpp"
#include "debugger.hpp"
#include "token.hpp"
#include "traits_manager.hpp"
#include "parser_helper.hpp"
#include "static_data.hpp"

Parser::Parser():
    mIndex(0),
    mTraitsManager(nullptr),
    mHelper(nullptr),
    mIsValid(true)
{
    mHelper = new ParserHelper(this);
    mTraitsManager = new TraitsManager();
}

Parser::~Parser()
{
    delete mTraitsManager;
    delete mHelper;
}

bool Parser::operator()()
{
    parse();

    Debugger::parser(DATA::PARSER_DATA());
    return mIsValid;
}

void Parser::parse()
{
    DATA::PARSER_DATA() = unit();
    
    for(auto&& e : mTraitsManager->mFunctionTraits)
    {
        DATA::FUNCTION_TRAITS()
            .emplace_back(DATA::FunctionTrait{e.name,
                                              e.argsType});
    }
}

Token* Parser::unit()
{
    ParentToken* parTok
        = new ParentToken();

    while(mIndex < DATA::TOKENIZER_DATA().size() && mIsValid)
          parTok->children.push_back(function());

    return parTok;
}

Token* Parser::function()
{
    Token* token = nullptr;

    if(DATA::TOKENIZER_DATA().at(mIndex)->isDeclaration())
    {
        Token::Type* type = mHelper->getDeclaratedType();

        if(!isErrored(Token::VARIABLE))
            return token;

        VariableToken* varTok
            = Token::cast<VariableToken*>(DATA::TOKENIZER_DATA().at(mIndex - 1));
        FunctionToken* funTok
            = new FunctionToken(varTok->name, type);
        std::vector<Token::Type*> argsType;

        mTraitsManager->addFunctionTrait(funTok);
        mTraitsManager->incScope();
        bool isPrototype = false;
        mHelper->setArgsType(argsType, isPrototype);
        funTok->argsType = argsType;
        mTraitsManager->addFunctionArgsTrait(funTok, isPrototype);

        if(isPrototype)
        {
            isErrored(Token::END);
            return nullptr;
        }

        funTok->proc = block();
        funTok->offset = mTraitsManager->mOffsetCount;

        mTraitsManager->decScope();
        mTraitsManager->mOffsetCount = 0;

        token = funTok;
    }
    else
        isErrored(Token::DEC_INT);
    
    return token;
}

Token* Parser::block()
{
    ParentToken* parTok
        = new ParentToken();

    if(!isErrored(Token::OPEN_BLOCK))
        return nullptr;
    
    mTraitsManager->incScope();
    while(1)
    {
        if(isConsumed(Token::CLOSE_BLOCK) ||
           !mIsValid)
            break;
        
        if(isValid(Token::OPEN_BLOCK))
            parTok->children.push_back(block());
        else
            parTok->children.push_back(statement());
    }
    mTraitsManager->decScope();

    return parTok;
}

Token* Parser::statement()
{
    Token* token = nullptr;

    // while
    if(DATA::TOKENIZER_DATA().at(mIndex)->isWhile())
    {
        WhileToken* whiTok
            = Token::cast<WhileToken*>(DATA::TOKENIZER_DATA().at(mIndex++));
        
        if(!isErrored(Token::OPEN_BRACKET))
            return nullptr;
        
        mTraitsManager->incScope();

        whiTok->cmp = expression();
        
        if(!isErrored(Token::CLOSE_BRACKET))
            return nullptr;
        
        if(isValid(Token::OPEN_BLOCK))
            whiTok->proc = block();
        else
            whiTok->proc = statement();

        mTraitsManager->decScope();

        token = whiTok;
        return token;
    }
    // for
    else if(DATA::TOKENIZER_DATA().at(mIndex)->isFor())
    {
        ForToken* forTok
            = Token::cast<ForToken*>(DATA::TOKENIZER_DATA().at(mIndex++));

        if(!isErrored(Token::OPEN_BRACKET))
            return nullptr;

        mTraitsManager->incScope();

        forTok->init = declaration();
        if(!isErrored(Token::END))
            return nullptr;
        forTok->cmp = expression();
        if(!isErrored(Token::END))
            return nullptr;
        forTok->rate = expression();
        
        if(!isErrored(Token::CLOSE_BRACKET))
            return nullptr;
        
        if(isValid(Token::OPEN_BLOCK))
            forTok->proc = block();
        else
            forTok->proc = statement();
        
        mTraitsManager->decScope();

        token = forTok;
        return token;
    }
    // if
    else if(DATA::TOKENIZER_DATA().at(mIndex)->isIf())
    {
        IfToken* ifTok
            = Token::cast<IfToken*>(DATA::TOKENIZER_DATA().at(mIndex++));
        
        if(!isErrored(Token::OPEN_BRACKET))
            return nullptr;

        mTraitsManager->incScope();

        Token* cmp = expression();
        Token* proc  = nullptr;
        
        if(!isErrored(Token::CLOSE_BRACKET))
            return nullptr;
        if(isValid(Token::OPEN_BLOCK))
            proc = block();
        else
            proc = statement();

        ifTok->children.emplace_back(IfToken::IfChild{cmp, proc});

        while(isConsumed(Token::ELSE))
        {
            if(isConsumed(Token::IF))
            {
                if(!isErrored(Token::OPEN_BRACKET))
                    return nullptr;
                
                cmp = expression();
                if(!isErrored(Token::CLOSE_BRACKET))
                    return nullptr;
                
                if(isValid(Token::OPEN_BLOCK))
                    proc = block();
                else
                    proc = statement();

                ifTok->children.emplace_back(IfToken::IfChild{cmp, proc});
            }
            else
            {
                cmp = nullptr;
                if(isValid(Token::OPEN_BLOCK))
                    proc = block();
                else
                    proc = statement();

                ifTok->children.emplace_back(IfToken::IfChild{cmp, proc});
                break;
            }
        }

        mTraitsManager->decScope();
        
        token = ifTok;
        return token;
    }
    // return
    else if(DATA::TOKENIZER_DATA().at(mIndex)->isReturn())
    {
        ReturnToken* retTok
            = Token::cast<ReturnToken*>(DATA::TOKENIZER_DATA().at(mIndex++));

        retTok->expr = expression();
        token = retTok;
    }
    else if(isValid(Token::END))
        token = nullptr;
    else
        token = declaration();

    if(!isErrored(Token::END))
        token = nullptr;

    return token;
}

Token* Parser::declaration()
{
    Token* token = nullptr;

    if(DATA::TOKENIZER_DATA().at(mIndex)->isDeclaration())
    {
        ParentToken* parTok = new ParentToken();
        
        Token::Type* type = mHelper->getDeclaratedType();

        while(1)
        {
            if(!isErrored(Token::VARIABLE))
                break;
            
            VariableToken* varTok
                = Token::cast<VariableToken*>(DATA::TOKENIZER_DATA().at(--mIndex));
            varTok->type = type;

            mTraitsManager->addVariableTrait(varTok);

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

        token = parTok;
    }
    else
        token = expression();

    return token;
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
            = Token::cast<OperatorToken*>(DATA::TOKENIZER_DATA().at(mIndex++));
        
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
                = Token::cast<OperatorToken*>(DATA::TOKENIZER_DATA().at(mIndex++));
            
            opeTok->lhs = token;
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
                = Token::cast<OperatorToken*>(DATA::TOKENIZER_DATA().at(mIndex++));
            
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
                = Token::cast<OperatorToken*>(DATA::TOKENIZER_DATA().at(mIndex++));

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
                = Token::cast<OperatorToken*>(DATA::TOKENIZER_DATA().at(mIndex++));
            
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
            = Token::cast<OperatorToken*>(DATA::TOKENIZER_DATA().at(mIndex++));

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

    // 変数, 関数
    if(isValid(Token::VARIABLE))
    {
        VariableToken* varTok
            = Token::cast<VariableToken*>(DATA::TOKENIZER_DATA().at(mIndex++));

        if(isConsumed(Token::OPEN_BRACKET))
        {
            CallToken* calTok
                = new CallToken(varTok->name);
            while(1)
            {
                calTok->args.push_back(expression());

                if(isConsumed(Token::COMMA))
                    continue;
                else
                    break;
            }

            isErrored(Token::CLOSE_BRACKET);
            mTraitsManager->setFunctionTrait(calTok);
            token = calTok;
        }
        else
        {
            mTraitsManager->setVariableTrait(varTok);
            token = varTok;
        }
    }
    // 整数値
    else if(isValid(Token::INTEGRAL))
    {
        token = DATA::TOKENIZER_DATA().at(mIndex++);
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
    if(mIndex >= DATA::TOKENIZER_DATA().size())
        return false;

    if(kind == DATA::TOKENIZER_DATA().at(mIndex)->kind)
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
              << ", fact: " << Token::KIND_NAME_MAP.at(DATA::TOKENIZER_DATA().at(mIndex)->kind)
              << ", index :" << mIndex
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