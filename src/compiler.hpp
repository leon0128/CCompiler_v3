#pragma once

#include <string>
#include <iostream>
#include <vector>

class Compiler
{
public:
    Compiler();
    ~Compiler();

    // コマンドライン引数をそのまま引数に取る
    bool operator()(int argc, char** argv);

private:
    // エラー出力
    bool error(const char* message);

    class Preprocessor* mPreprocessor;
    class Tokenizer* mTokenizer;
    class Parser* mParser;
    
    std::string mSource;
    std::vector<class Token*> mTokens;
    class Token* mParent;

    bool mIsValid;
};