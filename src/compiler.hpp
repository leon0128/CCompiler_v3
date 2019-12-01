#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

class Compiler
{
private:
    // 出力ファイル名
    static const char* RESULT_FILENAME;

public:
    Compiler();
    ~Compiler();

    // コマンドライン引数をそのまま引数に取る
    bool operator()(int argc, char** argv);

private:
    // エラー出力
    bool error(const char* message);

    class Preprocessor* mPreprocessor; // Preprocessor class
    class Tokenizer*    mTokenizer;    // Tokenizer class
    class Parser*       mParser;       // Parser class
    class Generator*    mGenerator;    // Generator class
    class Assembler*    mAssembler;    // Assembler class
    
    std::string               mSource;   // source code
    std::vector<class Token*> mTokens;   // token array
    class Token*              mParent;   // parent token
    std::stringstream         mAssembly; // assembly

    bool mIsValid;
};