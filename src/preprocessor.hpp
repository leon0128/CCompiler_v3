#pragma once

/* 実装状況
* - argv[1] のファイルの対してのみ処理
* - コメント の 削除
*/

#include <string>
#include <iostream>

class Preprocessor
{
public:
    Preprocessor();
    ~Preprocessor();

    // 第一引数, 第二引数は コマンドライン引数
    // 第三引数は 処理したソースコード
    bool operator()(int argc, char** argv);

private:
    // preprocess() で使用される関数
    bool isValidArgument(int argc); // コマンドライン引数が有効か
    bool isOpenedFile(char** argv); // ファイル読み込みの有無
    bool isDeletedComment();        // コメントの削除が有効か

    bool mIsValid;
};