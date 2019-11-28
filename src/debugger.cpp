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

void Debugger::preprocessor(const std::string& source)
{
    FileManager::write(PREPROCESS_FILENAME,
                       source.c_str());
}

void Debugger::tokenizer(const std::vector<Token*>& tokens)
{
    using namespace boost::property_tree;

    ptree tree;

    for(std::size_t i = 0; i < tokens.size(); i++)
        tree.put(std::to_string(i), Token::KIND_NAME_MAP.at(tokens.at(i)->kind));
    
    write_json(TOKENIZER_FILENAME,
               tree);
}

void Debugger::parser(Token* parent)
{
    ParserDebugger::debug(parent, PARSER_FILENAME);
}