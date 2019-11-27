#pragma once

/* 実装状況
* - argv[1] のファイルの対してのみ処理
* - コメント の 削除
*/

#include <string>
#include <iostream>

class Preprocessor
{
private:
    static const char* OUTPUT_FILENAME; // プリプロセスの結果を出力するファイル名

public:
    Preprocessor();
    ~Preprocessor();

    // 第一引数, 第二引数は コマンドライン引数
    // 第三引数は 処理したソースコード
    bool preprocess(int argc, char** argv,
                    std::string& source);

private:
    // preprocess() で使用される関数
    bool isValidArgument(int argc);
    bool isOpenedFile(char** argv);
    bool isDeletedComment();
    void outputData() const;

    std::string mSource;
    bool mIsValid;
};