#pragma once

#include "token.hpp"
#include <vector>
#include <iostream>

class Parser
{
public:
    Parser();
    ~Parser();

    bool operator()();

private:
    void parse();

    // parse() で使用する関数
    class Token* unit();           // ファイル全体
    class Token* function();       // 関数
    class Token* block();          // ブロック
    class Token* statement();      // 文
    class Token* declaration();    // 宣言
    class Token* expression();     // 式
    class Token* assignment();     // 代入
    class Token* equality();       // 比較(等)
    class Token* comparison();     // 比較(大小)
    class Token* addition();       // 加減
    class Token* multiplication(); // 乗除余
    class Token* unary();          // 単項
    class Token* primary();        // 変数、数値、() 

    bool isValid(Token::EKind kind) const;
    bool isConsumed(Token::EKind kind);
    bool isErrored(Token::EKind kind);

    bool error(Token::EKind kind);
    bool error(const char* message);

    // 関数の引数の型を格納する
    bool setArgsType(std::vector<Token::Type*>& argsType,
                     bool& isPrototype);

    std::size_t mIndex;
    class TraitsManager* mTraitsManager;
    bool mIsValid;
};