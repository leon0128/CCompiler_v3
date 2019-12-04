#include "static_data.hpp"

std::vector<DATA::FunctionTrait> functionTraits;
std::string preprocessorData;
std::vector<Token*> tokenizerData;
Token* parserData;
std::stringstream generatorData;

std::vector<DATA::FunctionTrait>& DATA::FUNCTION_TRAITS()
    {return functionTraits;}
