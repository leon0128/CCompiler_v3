#include "traits_manager.hpp"

TraitsManager::TraitsManager():
    mFunctionTraits(),
    mVariableTraits(),
    mScope(0),
    mOffsetCount(0)
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
        if(iter->name == funTok->name)
            return error(token, "function overloading.");
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
                                               funTok->argsType});
    return true;
}

bool TraitsManager::addVariableTrait(Token* token)
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

    mOffsetCount += 8;
    mVariableTraits.push_back(VariableTrait{mScope,
                                            varTok->type,
                                            varTok->name,
                                            static_cast<long>((mVariableTraits.size() + 1) * 8)});
    return setVariableTrait(token);
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

    std::cerr << "trai-err: " << message
              << " ( \"" << name << "\" )."
              << std::endl;
            
    return false;
}