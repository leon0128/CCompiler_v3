#include "traits_manager.hpp"

TraitsManager::TraitsManager():
    mTraits(),
    mScope(0)
{
}

TraitsManager::~TraitsManager()
{
}

void TraitsManager::decScope()
{
    while(1)
    {
        if(mTraits.back().scope == mScope)
            mTraits.pop_back();
        else
            break;
    }

    mScope--;
}

bool TraitsManager::addTrait(Token* token, Token::EType type)
{
    VariableToken* varTok
        = Token::cast<VariableToken*>(token);

    for(auto&& e : mTraits)
    {
        if(varTok->name == e.name)
            return error(token, "variable overloading");
    }

    mTraits.push_back(Trait{mScope,
                            type,
                            varTok->name,
                            static_cast<long>((mTraits.size() + 1) * 8)});
    return setTrait(token);
}

bool TraitsManager::setTrait(Token* token) const
{
    VariableToken* varTok
        = Token::cast<VariableToken*>(token);

    for(auto&& e : mTraits)
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
              << " ( " << varTok->name << " )."
              << std::endl;
            
    return false;
}