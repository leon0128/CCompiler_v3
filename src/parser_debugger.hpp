#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

// 各トークンのメンバを表示するかどうか
#define DEBUG_PARENT   1
#define DEBUG_OPERATOR 1
#define DEBUG_INTEGRAL 1

class ParserDebugger
{
public:
    ParserDebugger();
    ~ParserDebugger();

    bool debug(const char* filename,
               class Token* parent);

private:
    std::string consume(class Token* token,
                        std::string disc = std::string());
    std::string conParent(class Token* token,
                          std::string disc = std::string());
    std::string conOperator(class Token* token,
                            std::string disc = std::string());
    std::string conIntegral(class Token* token,
                            std::string disc = std::string());

    // [-] [KIND_NAME]\n をストリームに追加
    void addNodeHeader(class Token* token,
                       std::stringstream& stream,
                       std::string& disc);
    // indent の作成
    std::string createIndent(std::size_t size);

    bool error(class Token* token);

    std::vector<bool> mIsValidIndents;
};