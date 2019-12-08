#pragma once

#include "token.hpp"
#include <vector>

class ParserHelper
{
public:
    ParserHelper(class Parser* parser);

    // 関数の引数の型を格納する
    bool setArgsType(std::vector<Token::Type*>& argsType,
                     bool& isPrototype);

    // 型宣言から型を取得する
    Token::Type* getDeclaratedType();

private:
    class Parser* mParser;
};