#pragma once

#include <string>
#include <vector>
#include <sstream>

class Debugger
{
private:
    // 各デバックデータの出力ファイル名
    static const char* PREPROCESS_FILENAME;
    static const char* TOKENIZER_FILENAME;
    static const char* PARSER_FILENAME;
    static const char* GENERATOR_FILENAME;

public:
    Debugger() = delete;

    // 各工程での処理結果を出力
    static void preprocessor(const std::string& source);
    static void tokenizer(const std::vector<class Token*>& tokens);
    static void parser(class Token* parent);
    static void generator(const std::stringstream& stream);

private:
    // error
    static bool error(const char* section);
};