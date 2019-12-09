#include "traits_manager.hpp"
#include "static_data.hpp"

TraitsManager::TraitsManager():
    mFunctionTraits(),
    mVariableTraits(),
    mScope(0),
    mOffsetCount(0),
    mLocalOffset(0)
{
}

TraitsManager::~TraitsManager()
{
}

void TraitsManager::decScope()
{
    while(!mVariableTraits.empty())
    {
        if(mVariableTraits.back().scope == mScope)
            mVariableTraits.pop_back();
        else
            break;
    }

    mScope--;
}

bool TraitsManager::addFunctionTrait(Token* token)
{
    FunctionToken* funTok
        = Token::cast<FunctionToken*>(token);
    
    for(auto iter = mFunctionTraits.begin();
        iter != mFunctionTraits.end();
        iter++)
    {
        if(iter->name == funTok->name && iter->isDefined)
            return error(token, "function overloading.");
        else if(iter->name == funTok->name)
            return true;
    }
    for(auto iter = mVariableTraits.begin();
        iter != mVariableTraits.end();
        iter++)
    {
        if(iter->name == funTok->name)
            return error(token, "variable with the same name exists.");
    }

    mFunctionTraits.emplace_back(FunctionTrait{funTok->type,
                                               funTok->name,
                                               funTok->argsType,
                                               false});
    return true;
}

bool TraitsManager::addFunctionArgsTrait(Token* token, bool isPrototype)
{
    FunctionToken* funTok
        = Token::cast<FunctionToken*>(token);

    for(auto&& e : mFunctionTraits)
    {
        if(e.name == funTok->name)
        {
            e.isDefined = !isPrototype;
            e.argsType = funTok->argsType;
            return true;
        }
    }

    return false;
}

bool TraitsManager::addVariableTrait(Token* token, bool isLocal, long argIndex)
{
    VariableToken* varTok
        = Token::cast<VariableToken*>(token);

    if(mScope == 0)
    {
        for(auto iter = mFunctionTraits.begin();
            iter != mFunctionTraits.end();
            iter++)
        {
            if(iter->name == varTok->name)
                return error(token, "function with the same name exists.");
        }
    }
    for(auto iter = mVariableTraits.rbegin();
        iter != mVariableTraits.rend();
        iter++)
    {
        if(iter->scope == mScope)
        {
            if(iter->name == varTok->name)
                return error(token, "variable overloading");
        }
        else
            break;
    }
    
    if(isLocal)
    {
        mOffsetCount += 8;
        mLocalOffset += 8;
        mVariableTraits.push_back(VariableTrait{mScope,
                                                varTok->type,
                                                varTok->name,
                                                mLocalOffset,
                                                !isLocal});
    }
    else
    {
        mVariableTraits.push_back(VariableTrait{mScope,
                                                varTok->type,
                                                varTok->name,
                                                argIndex,
                                                !isLocal});
    }
    varTok->isArg = !isLocal;
    return setVariableTrait(token);
}

bool TraitsManager::setFunctionTrait(Token* token) const
{
    CallToken* calTok
        = Token::cast<CallToken*>(token);

    for(auto&& e : mFunctionTraits)
    {
        if(e.name == calTok->name)
        {
            if(e.argsType.size() != calTok->args.size())
                return error(token, "invalid number of function call arguments");
        
            calTok->type = e.type;
            return true;
        }
    }

    return error(token, "undefined function call");
}

bool TraitsManager::setVariableTrait(Token* token) const
{
    VariableToken* varTok
        = Token::cast<VariableToken*>(token);

    for(auto&& e : mVariableTraits)
    {
        if(varTok->name == e.name)
        {
            varTok->type   = e.type;
            varTok->offset = e.offset;
            varTok->isArg  = e.isArg;
            return true;
        }
    }

    return error(token, "undefined variable");
}

bool TraitsManager::error(Token* token, const char* message) const
{
    std::string name;

    if(token->isVariable())
    {
        VariableToken* varTok
            = Token::cast<VariableToken*>(token);
        name = varTok->name;
    }
    else if(token->isFunction())
    {
        FunctionToken* funTok
            = Token::cast<FunctionToken*>(token);
        name = funTok->name;
    }
    else if(token->isCall())
    {
        CallToken* calTok
            = Token::cast<CallToken*>(token);
        name = calTok->name;
    }

    std::cerr << "trai-err: " << message
              << " ( \"" << name << "\" )."
              << std::endl;
            
    return false;
}