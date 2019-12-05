#pragma once

#include "token.hpp"
#include <vector>
#include <string>
#include <sstream>

namespace DATA
{

// 関数の特性 を纏めた 構造体
struct FunctionTrait
{
    std::string name; // 関数名
};

// 関数特性を格納するコンテナ
std::vector<FunctionTrait>& FUNCTION_TRAITS();

// preprocessor class の実行結果を格納するコンテナ
std::string& PREPROCESSOR_DATA();

// tokenizer class の実行結果を格納するコンテナ
std::vector<Token*>& TOKENIZER_DATA();

// parser class の実行結果を格納するデータ
Token*& PARSER_DATA();

// generator class の実行結果を格納するストリーム
std::stringstream& GENERATOR_DATA();
};