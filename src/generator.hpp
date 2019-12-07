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
        MOV, MOVZX, MOVSX,// 代入
        PUSH, POP,        // スタック操作
        CMP, JMP, JNE,    // 比較 ジャンプ
        SETE, SETNE, SETL, SETLE, SETG, SETGE, // フラグ
        ADD, SUB, IMUL, IDIV, // 四則演算 (符号付)
        CQO, CDQE,  // 拡張
        CALL, RET  // 関数
    };
    // <[命令文字列], [引数の個数]> を 値として持つ マップ
    static const std::unordered_map<EInstruction, std::pair<const char*, std::size_t>> INSTRUCTION_MAP;

public:
    Generator();
    ~Generator();

    bool operator()();
    
private:
    void generate();

    void consume(class Token* token);
    void conParent(class Token* token);
    void conFunction(class Token* token);
    void conCall(class Token* token);
    void conWhile(class Token* token);
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

    bool mIsValid;
};