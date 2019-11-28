#pragma once

#include <string>
#include <sstream>
#include "operand.hpp"

class Generator
{
    // 命令一覧
    enum EInstruction
    {
        MOV, // 代入
        PUSH, POP, // スタック操作
        RET // 関数
    };

public:
    Generator();
    ~Generator();

    bool operator()(class Token* parent,
                    std::stringstream& assembly);
    
private:
    void generate();

    void consume(class Token* token);
    void conOperator(class Token* token);
    void conIntegral(class Token* token);

    void instruction(EInstruction inst,
                     Operand ope1 = Operand(),
                     Operand ope2 = Operand(),
                     Operand ope3 = Operand(),
                     Operand ope4 = Operand());

    class Token* mParent;
    std::stringstream mAssembly;
    bool mIsValid;
};