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
    std::string data(consume(parent));

    return FileManager::write(filename, data);
}

std::string ParserDebugger::consume(Token* token, std::string disc)
{
    std::string data;

    if(token)
    {
        if(token->isOperator())
            data = conOperator(token, disc);
        else if(token->isIntegral())
            data = conIntegral(token, disc);
        else
            error(token);
    }

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

std::string ParserDebugger::conIntegral(Token* token, std::string disc)
{    
    std::stringstream stream;
    addNodeHeader(token, stream, disc);

    #if DEBUG_INTEGRAL
    IntegralToken* intTok
        = Token::cast<IntegralToken*>(token);
    stream << createIndent(mIsValidIndents.size())
           << " `----- "
           << intTok->value
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