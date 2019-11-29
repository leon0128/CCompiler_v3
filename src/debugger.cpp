#include "debugger.hpp"
#include "file_manager.hpp"
#include "token.hpp"
#include "parser_debugger.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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
    if(!FileManager::write(PREPROCESS_FILENAME,
                           source.c_str()))
        error("preprocessor");
}

void Debugger::tokenizer(const std::vector<Token*>& tokens)
{
    using namespace boost::property_tree;

    ptree tree;

    for(std::size_t i = 0; i < tokens.size(); i++)
        tree.put(std::to_string(i), Token::KIND_NAME_MAP.at(tokens.at(i)->kind));
    
    if(!FileManager::write(TOKENIZER_FILENAME,
                           &tree))
        error("tokenizer");
}

void Debugger::parser(Token* parent)
{
    if(!ParserDebugger::debug(parent, PARSER_FILENAME))
        error("parser");
}

void Debugger::generator(const std::stringstream& stream)
{
    std::string temp(stream.str());

    if(!FileManager::write(GENERATOR_FILENAME,
                           temp.c_str()))
        error("generator");
}

bool Debugger::error(const char* section)
{
    std::cerr << "debu-err: failed to write debug data "
              << "( " << section << " )." << std::endl;
    return false;
}