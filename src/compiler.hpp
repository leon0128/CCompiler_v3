#pragma once

class Compiler
{
public:
    Compiler();
    ~Compiler();

    // コマンドライン引数をそのまま引数に取る
    bool operator()(int argc, char** argv);

private:
    class Preprocessor* mPreprocessor;
};