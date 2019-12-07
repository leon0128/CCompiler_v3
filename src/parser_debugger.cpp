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
        else if(token->isCall())
            data = conCall(token, disc);
        else if(token->isWhile())
            data = conWhile(token, disc);
        else if(token->isFor())
            data = conFor(token, disc);
        else if(token->isIf())
            data = conIf(token, disc);
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
    else
        data = conNullptr(disc);

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
               << std::endl;
        
        for(std::size_t i = 0; i < funTok->argsType.size(); i++)
        {
            stream << createIndent(mIsValidIndents.size())
                   << " |----- "
                   << Token::TYPE_NAME_MAP.at(funTok->argsType.at(i))
                   << " (arg " << i << ")" << std::endl;
        }
        
        stream << createIndent(mIsValidIndents.size())
               << " |--[-] (ARGS [not token])" << std::endl;
        mIsValidIndents.push_back(true);
        for(std::size_t i = 0; i < funTok->argsType.size(); i++)
        {
            stream << createIndent(mIsValidIndents.size());
            if(i != funTok->argsType.size() - 1)
                stream << " |----- ";
            else
                stream << " `----- ";
            
            stream << Token::TYPE_NAME_MAP.at(funTok->argsType.at(i))
                   << " (arg " << i << ")" << std::endl;
        }
        mIsValidIndents.pop_back();

        stream << createIndent(mIsValidIndents.size())
               << " |----- "
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

std::string ParserDebugger::conCall(Token* token, std::string disc)
{
    std::stringstream stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_CALL
        CallToken* calTok
            = Token::cast<CallToken*>(token);

        stream << createIndent(mIsValidIndents.size())
               << " |----- "
               << Token::TYPE_NAME_MAP.at(calTok->type)
               << " (type)" << std::endl
               << createIndent(mIsValidIndents.size())
               << " |----- "
               << "\"" << calTok->name << "\""
               << " (name)" << std::endl;
        
        ParentToken* parTok
            = new ParentToken();
        parTok->children = calTok->args;
        
        mIsValidIndents.push_back(false);
        stream << createIndent(mIsValidIndents.size() - 1)
               << " `--"
               << consume(parTok, "args");
        mIsValidIndents.pop_back();
    #endif

    std::string data(stream.str());
    return data;
}

std::string ParserDebugger::conWhile(Token* token, std::string disc)
{
    std::stringstream stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_WHILE
        WhileToken* whiTok
            = Token::cast<WhileToken*>(token);

        stream << createIndent(mIsValidIndents.size())
               << " |----- "
               << whiTok->label
               << " (label)" << std::endl;

        mIsValidIndents.push_back(true);
        stream << createIndent(mIsValidIndents.size() - 1)
               << " |--"
               << consume(whiTok->cmp, "cmp");
        mIsValidIndents.back() = false;
        stream << createIndent(mIsValidIndents.size() - 1)
               << " `--"
               << consume(whiTok->proc, "proc");
        mIsValidIndents.pop_back();
    #endif

    std::string data(stream.str());
    return data;
}

std::string ParserDebugger::conFor(Token* token, std::string disc)
{
    std::stringstream  stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_FOR
        ForToken* forTok
            = Token::cast<ForToken*>(token);

        stream << createIndent(mIsValidIndents.size())
               << " |----- "
               << forTok->label
               << " (label)" << std::endl;

        mIsValidIndents.push_back(true);
        stream << createIndent(mIsValidIndents.size() - 1)
               << " |--"
               << consume(forTok->init, "init")
               << createIndent(mIsValidIndents.size() - 1)
               << " |--"
               << consume(forTok->cmp, "cmp")
               << createIndent(mIsValidIndents.size() - 1)
               << " |--"
               << consume(forTok->rate, "rate");
        mIsValidIndents.back() = false;
        stream << createIndent(mIsValidIndents.size() - 1)
               << " `--"
               << consume(forTok->proc, "proc");
        mIsValidIndents.pop_back();
    #endif

    std::string data(stream.str());
    return data;
}

std::string ParserDebugger::conIf(Token* token, std::string disc)
{
    std::stringstream stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_IF
        IfToken* ifTok
            = Token::cast<IfToken*>(token);

        stream << createIndent(mIsValidIndents.size())
               << " |----- "
               << ifTok->label
               << " (label)\n"
               << createIndent(mIsValidIndents.size())
               << " `--[-] (CHILDREN [not token])"
               << std::endl;
        
        mIsValidIndents.push_back(false);
        mIsValidIndents.push_back(true);
        for(std::size_t i = 0; i < ifTok->children.size(); i++)
        {            
            if(i == ifTok->children.size() - 1)
            {
                mIsValidIndents.back() = false;
                stream << createIndent(mIsValidIndents.size() - 1)
                       << " `--[-] (IF_CHILD [not token])"
                       << std::endl;
            }
            else
            {
                stream << createIndent(mIsValidIndents.size() - 1)
                       << " |--[-] (IF_CHILD [not token])"
                       << std::endl;
            }
            mIsValidIndents.push_back(true);
            stream << createIndent(mIsValidIndents.size() - 1)
                   << " |-----"
                   << i << " (index)" << std::endl;
            mIsValidIndents.back() = true;
            stream << createIndent(mIsValidIndents.size() - 1)
                   << " |--"
                   << consume(ifTok->children.at(i).cmp, "cmp");
            mIsValidIndents.back() = false;
            stream << createIndent(mIsValidIndents.size() - 1)
                   << " `--"
                   << consume(ifTok->children.at(i).proc, "proc");
            mIsValidIndents.pop_back();
        }
        mIsValidIndents.pop_back();
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
               << varTok->offset;

        if(varTok->isArg)
            stream << " (argIndex)" << std::endl;
        else
            stream << " (offset)" << std::endl;
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

std::string ParserDebugger::conNullptr(std::string disc)
{
    std::string data("[-] NULL (");
    data += disc;
    data += ")\n";
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