#include "traits_manager.hpp"

TraitsManager::TraitsManager():
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

bool TraitsManager::addVariableTrait(Token* token, Token::EType type)
{
    VariableToken* varTok
        = Token::cast<VariableToken*>(token);

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
                                            type,
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
    VariableToken* varTok
        = Token::cast<VariableToken*>(token);

    std::cerr << "trai-err: " << message
              << " ( \"" << varTok->name << "\" )."
              << std::endl;
            
    return false;
}