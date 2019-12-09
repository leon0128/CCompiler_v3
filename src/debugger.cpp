#include "debugger.hpp"
#include "token.hpp"
#include "file_manager.hpp"
#include "parser_debugger.hpp"

const char* Debugger::PREPROCESS_FILENAME
    = "debug/preprocessor.log";
const char* Debugger::TOKENIZER_FILENAME
    = "debug/tokenizer.log";
const char* Debugger::PARSER_FILENAME
    = "debug/parser.log";
const char* Debugger::GENERATOR_FILENAME
    = "debug/generator.log";

void Debugger::preprocessor(const std::string& source)
{
    if(!FileManager::write(PREPROCESS_FILENAME, source))
        error("preprocessor");
}

void Debugger::tokenizer(const std::vector<Token*>& tokens)
{
    std::stringstream stream;

    stream << "{" << std::endl;
    for(std::size_t i = 0; i < tokens.size(); i++)
    {
        stream << std::string(4, ' ') << i << ": "
               << Token::KIND_NAME_MAP.at(tokens.at(i)->kind);
        if(i != tokens.size() - 1)
            stream << "," << std::endl;
    }
    stream << std::endl << "}" << std::endl;

    if(!FileManager::write(TOKENIZER_FILENAME, stream))
        error("tokenizer");
}

void Debugger::parser(Token* parent)
{
    ParserDebugger debugger;
    if(!debugger.debug(PARSER_FILENAME, parent))
        error("parser");
}

void Debugger::generator(const std::stringstream& stream)
{
    if(!FileManager::write(GENERATOR_FILENAME, stream))
        error("generator");
}

bool Debugger::error(const char* section)
{
    std::cerr << "debu-err: failed to write debug data "
              << "( " << section << " )." << std::endl;
    return false;
}