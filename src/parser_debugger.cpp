#include "parser_debugger.hpp"
#include "file_manager.hpp"
#include "token.hpp"

ParserDebugger::ParserDebugger():
    mStream(),
    mIndent(0)
{
}

bool ParserDebugger::debug(const char* filename,
                           Token* parent)
{
    consume(parent);

    return FileManager::write(filename, mStream);
}

void ParserDebugger::consume(Token* token)
{
    if(!token)
        return;

    if(token->isIntegral())
        conIntegral(token);
    else if(token->isOperator())
        conOperator(token);
    else
        error(token);
}

void ParserDebugger::conIntegral(Token* token)
{
    IntegralToken* intTok
        = Token::cast<IntegralToken*>(token);
    

}

void ParserDebugger::conOperator(Token* token)
{
    OperatorToken* opeTok
        = Token::cast<OperatorToken*>(token);


}

bool ParserDebugger::error(Token* token)
{
    std::cerr << "debu-err: unimplemented type "
              << "( " << Token::KIND_NAME_MAP.at(token->kind) << " )."
              << std::endl;
}