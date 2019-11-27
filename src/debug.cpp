#include "debug.hpp"
#include "file_manager.hpp"
#include "token.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

const char* Debug::PREPROCESS_FILENAME
    = "debug/preprocessor.log";
const char* Debug::TOKENIZER_FILENAME
    = "debug/tokenizer.log";
const char* Debug::PARSER_FILENAME
    = "debug/parser.log";

void Debug::preprocessor(const std::string& source)
{
    FileManager::write(PREPROCESS_FILENAME,
                       source.c_str());
}

void Debug::tokenizer(const std::vector<Token*>& tokens)
{
    using namespace boost::property_tree;

    ptree tree;

    for(std::size_t i = 0; i < tokens.size(); i++)
        tree.put(std::to_string(i), Token::KIND_NAME_MAP.at(tokens.at(i)->kind));
    
    write_json(TOKENIZER_FILENAME,
               tree);
}