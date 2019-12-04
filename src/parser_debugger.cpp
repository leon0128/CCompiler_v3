#include "parser_debugger.hpp"
#include "file_manager.hpp"
#include "token.hpp"

ParserDebugger::ParserDebugger():
    mIsValidIndents()
{
}

ParserDebugger::~ParserDebugger()
{
}

bool ParserDebugger::debug(const char* filename,
                           Token* parent)
{
    std::string data(consume(parent, "unit"));

    return FileManager::write(filename, data);
}

std::string ParserDebugger::consume(Token* token, std::string disc)
{
    std::string data;

    if(token)
    {
        if(token->isParent())
            data = conParent(token, disc);
        else if(token->isFunction())
            data = conFunction(token, disc);
        else if(token->isOperator())
            data = conOperator(token, disc);
        else if(token->isReturn())
            data = conReturn(token, disc);
        else if(token->isVariable())
            data = conVariable(token, disc);
        else if(token->isIntegral())
            data = conIntegral(token, disc);
        else
            error(token);
    }

    return data;
}

std::string ParserDebugger::conParent(Token* token, std::string disc)
{
    std::stringstream stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_PARENT
        ParentToken* parTok
            = Token::cast<ParentToken*>(token);
        mIsValidIndents.push_back(true);

        for(std::size_t i = 0; i < parTok->children.size(); i++)
        {
            if(i != parTok->children.size() - 1)
            {
                stream << createIndent(mIsValidIndents.size() - 1)
                       << " |--"
                       << consume(parTok->children.at(i), "childlen");
            }
            else
            {
                mIsValidIndents.back() = false;
                stream << createIndent(mIsValidIndents.size() - 1)
                       << " `--"
                       << consume(parTok->children.at(i), "children");
            }
        }

        mIsValidIndents.pop_back();
    #endif

    std::string data(stream.str());
    return data;
}

std::string ParserDebugger::conFunction(Token* token, std::string disc)
{
    std::stringstream stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_FUNCTION
        FunctionToken* funTok
            = Token::cast<FunctionToken*>(token);
        
        stream << createIndent(mIsValidIndents.size())
               << " |----- "
               << Token::TYPE_NAME_MAP.at(funTok->type) << " (type)"
               << std::endl
               << createIndent(mIsValidIndents.size())
               << " |----- \""
               << funTok->name << "\" (name)"
               << std::endl
               << createIndent(mIsValidIndents.size());
        
        for(std::size_t i = 0; i < funTok->argsType.size(); i++)
        {
            stream << createIndent(mIsValidIndents.size())
                   << " |----- "
                   << Token::TYPE_NAME_MAP.at(funTok->argsType.at(i))
                   << " (arg " << i << ")" << std::endl;
        }
        stream << " |----- "
               << funTok->offset << " (offset)"
               << std::endl;

        mIsValidIndents.push_back(false);
        stream << createIndent(mIsValidIndents.size() - 1)
               << " `--"
               << consume(funTok->proc, "proc");
        mIsValidIndents.pop_back();
    #endif

    std::string data(stream.str());
    return data;
}

std::string ParserDebugger::conOperator(Token* token, std::string disc)
{
    std::stringstream stream;
    addNodeHeader(token, stream, disc);
    
    #if DEBUG_OPERATOR
        OperatorToken* opeTok
            = Token::cast<OperatorToken*>(token);
        
        mIsValidIndents.push_back(true);
        stream << createIndent(mIsValidIndents.size() - 1)
               << " |--"
               << consume(opeTok->lhs, "lhs");
        mIsValidIndents.back() = false;
        stream << createIndent(mIsValidIndents.size() - 1)
               << " `--"
               << consume(opeTok->rhs, "rhs");
        mIsValidIndents.pop_back();
    #endif

    std::string data(stream.str());
    return data;
}

std::string ParserDebugger::conReturn(Token* token, std::string disc)
{
    std::stringstream stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_RETURN
        ReturnToken* retTok
            = Token::cast<ReturnToken*>(token);

        mIsValidIndents.push_back(false);
        stream << createIndent(mIsValidIndents.size() - 1)
               << " `--"
               << consume(retTok->expr, "expr");
        mIsValidIndents.pop_back();
    #endif

    std::string data(stream.str());
    return data;
}

std::string ParserDebugger::conVariable(Token* token, std::string disc)
{
    std::stringstream stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_VARIABLE
        VariableToken* varTok
            = Token::cast<VariableToken*>(token);

        stream << createIndent(mIsValidIndents.size())
               << " |----- "
               << Token::TYPE_NAME_MAP.at(varTok->type)
               << " (type)" << std::endl
               << createIndent(mIsValidIndents.size())
               << " |----- \""
               << varTok->name << "\" (name)"
               << std::endl
               << createIndent(mIsValidIndents.size())
               << " `----- "
               << varTok->offset << " (offset)"
               << std::endl;
    #endif

    std::string data(stream.str());
    return data;
}

std::string ParserDebugger::conIntegral(Token* token, std::string disc)
{    
    std::stringstream stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_INTEGRAL
        IntegralToken* intTok
            = Token::cast<IntegralToken*>(token);

        stream << createIndent(mIsValidIndents.size())
               << " `----- "
               << intTok->value << " (value)"
               << std::endl;
    #endif

    std::string data(stream.str());
    return data;
}

void ParserDebugger::addNodeHeader(Token* token,
                                   std::stringstream& stream,
                                   std::string& disc)
{
    stream << "[-] "
           << Token::KIND_NAME_MAP.at(token->kind)
           << " (" << disc << ")"
           << std::endl;
}

std::string ParserDebugger::createIndent(std::size_t size)
{
    std::string data;
    for(std::size_t i = 0; i < size; i++)
        data += mIsValidIndents.at(i) ? " |  " : "    ";
    
    return data;
}

bool ParserDebugger::error(Token* token)
{
    std::cerr << "debu-err: unimplemented type "
              << "( " << Token::KIND_NAME_MAP.at(token->kind) << " )."
              << std::endl;

    return false;
}