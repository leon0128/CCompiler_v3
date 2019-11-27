#include "debug.hpp"
#include "file_manager.hpp"
#include "token.hpp"

const char* Debug::PREPROCESS_FILENAME
    = "debug/preprocessor.log";
const char* Debug::TOKENIZER_FILENAME
    = "debug/tokenizer.log";

void Debug::preprocessor(const std::string& source)
{
    FileManager::write(PREPROCESS_FILENAME,
                       source.c_str());
}

void Debug::tokenizer(const std::vector<Token*>& tokens)
{
    std::string data("{\n");

    for(std::size_t i = 0; i < tokens.size(); i++)
    {
        data += "    " + std::to_string(i);
        data += std::string(": \"") + Token::KIND_NAME_MAP.at(tokens.at(i)->kind);
        data += "\"";

        if(i != tokens.size() - 1)
            data += ",";
        data += "\n";
    }

    data += "}\n";

    FileManager::write(TOKENIZER_FILENAME,
                       data.c_str());
}