#include "static_data.hpp"

std::vector<DATA::FunctionTrait> functionTraits;
std::string preprocessorData;
std::vector<Token*> tokenizerData;
Token* parserData;
std::stringstream generatorData;

std::vector<DATA::FunctionTrait>& DATA::FUNCTION_TRAITS()
    {return functionTraits;}
std::string& DATA::PREPROCESSOR_DATA()
    {return preprocessorData;}
std::vector<Token*>& DATA::TOKENIZER_DATA()
    {return tokenizerData;}
Token*& DATA::PARSER_DATA()
    {return parserData;}
std::stringstream& DATA::GENERATOR_DATA()
    {return generatorData;}