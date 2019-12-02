#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include "operand.hpp"

class Generator
{
    // 命令一覧
    enum EInstruction
    {
        MOV, MOVZX, // 代入
        PUSH, POP,  // スタック操作
        CMP,        // 比較
        SETE, SETNE, SETL, SETLE, SETG, SETGE, // フラグ
        ADD, SUB, IMUL, IDIV, // 四則演算 (符号付)
        CQO, // 拡張
        RET  // 関数
    };
    // <[命令文字列], [引数の個数]> を 値として持つ マップ
    static const std::unordered_map<EInstruction, std::pair<const char*, std::size_t>> INSTRUCTION_MAP;

public:
    Generator();
    ~Generator();

    bool operator()(class Token* parent,
                    std::stringstream& assembly);
    
private:
    void generate();

    void consume(class Token* token);
    void conParent(class Token* token);
    void conFunction(class Token* token);
    void conArithmeticOperator(class Token* token);
    void conAssignmentOperator(class Token* token);
    void conCompareOperator(class Token* token);
    void conReturn(class Token* token);
    void conVariable(class Token* token);
    void conIntegral(class Token* token);

    void instruction(EInstruction inst,
                     Operand ope1 = Operand(),
                     Operand ope2 = Operand(),
                     Operand ope3 = Operand(),
                     Operand ope4 = Operand());

    bool error(class Token* token);

    class Token* mParent;
    std::stringstream mAssembly;
    bool mIsValid;
};