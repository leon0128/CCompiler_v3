#pragma once

#include "token.hpp"
#include <vector>
#include <iostream>

class Parser
{
public:
    Parser();
    ~Parser();

    bool operator()(std::vector<class Token*>& tokens,
                    class Token*& parent);

private:
    void parse();

    // parse() で使用する関数
    class Token* unit();           // ファイル全体
    class Token* function();       // 関数
    class Token* statement();      // 文
    class Token* expression();     // 式
    class Token* assignment();     // 代入
    class Token* addition();       // 加減
    class Token* multiplication(); // 乗除余
    class Token* unary();          // 単項
    class Token* primary();        // 変数、数値、() 

    bool isValid(Token::EKind kind) const;
    bool isConsumed(Token::EKind kind);
    bool isErrored(Token::EKind kind);

    bool error(Token::EKind kind);
    bool error(const char* message);

    // 変数の情報を格納する構造体 と 配列
    struct VariableTrait
    {
        std::string name;
        long offset;
    };
    std::vector<VariableTrait> mVariableTraits;
    // 変数に値を設定
    void setVariableTrait(class Token* token);

    std::vector<class Token*> mTokens;
    class Token* mParent;
    std::size_t mIndex;
    bool mIsValid;
};