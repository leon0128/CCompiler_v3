#include "parser_helper.hpp"
#include "parser.hpp"
#include "token.hpp"
#include "traits_manager.hpp"
#include "static_data.hpp"

ParserHelper::ParserHelper(Parser* parser):
    mParser(parser)
{
}

bool ParserHelper::setArgsType(std::vector<Token::Type*>& argsType,
                               bool& isPrototype)
{
    isPrototype = false;

    if(!mParser->isErrored(Token::OPEN_BRACKET))
        return false;
    if(mParser->isConsumed(Token::CLOSE_BRACKET))
        return true;

    long argIndex = 0;
    while(1)
    {
        if(!DATA::TOKENIZER_DATA().at(mParser->mIndex)->isDeclaration())
            return mParser->isErrored(Token::DEC_INT);
        
        argsType.push_back(getDeclaratedType());

        if(mParser->isValid(Token::VARIABLE))
        {
            VariableToken* varTok
                = Token::cast<VariableToken*>(DATA::TOKENIZER_DATA().at(mParser->mIndex++));
            varTok->type = argsType.back();
            mParser->mTraitsManager->addVariableTrait(varTok, false, argIndex++);
        }
        else
            isPrototype = false;
    
        if(mParser->isConsumed(Token::COMMA))
            continue;
        else if(!mParser->isErrored(Token::CLOSE_BRACKET))
            return false;
        else
            break;
    }

    return true;
}

Token::Type* ParserHelper::getDeclaratedType()
{
    Token::Type* type
        = new Token::Type(Token::TYPE_DEC_MAP.at(DATA::TOKENIZER_DATA().at(mParser->mIndex++)->kind));

    while(mParser->isConsumed(Token::ASTERISK))
    {
        Token::Type* outer
            = new Token::Type(Token::POINTER);
        
        outer->node = type;
        type = outer;
    }

    return type;
}