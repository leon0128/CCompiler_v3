#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <array>
#include "token.hpp"

class Operand
{
private:
    static const std::unordered_map<long, const char*> SPECIFICATION_SIZE_MAP;

public:
    enum ERegister
    {
        // 64 bit
        RAX, RBX, RCX, RDX,
        RSI, RDI, RSP, RBP,
        R8, R9, R10, R11, R12, R13, R14, R15,
        // 32 bit
        EAX, EBX, ECX, EDX,
        ESI, EDI, ESP, EBP,
        R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D,
        // 16 bit
        AX, BX, CX, DX,
        SI, DI, SP, BP,
        R8W, R9W, R10W, R11W, R12W, R13W, R14W, R15W,
        // 上位 8 bit
        AH, BH, CH, DH,
        // 下位 8 bit
        AL, BL, CL, DL,
        SIL, DIL, SPL, BPL,
        R8L, R9L, R10L, R11L, R12L, R13L, R14L, R15L,
        // FPU
        R0, R1, R2, R3, R4, R5, R6, R7,
        // MMX
        MM0, MM1, MM2, MM3, MM4, MM5, MM6, MM7,
        // XMM
        XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7,
        XMM8, XMM9, XMM10, XMM11, XMM12, XMM13, XMM14, XMM15,
        // YMM
        YMM0, YMM1, YMM2, YMM3, YMM4, YMM5, YMM6, YMM7,
        YMM8, YMM9, YMM10, YMM11, YMM12, YMM13, YMM14, YMM15
    };
    static const std::unordered_map<ERegister, const char*> REGISTER_NAME_MAP;
    static const std::array<ERegister, 7> ARG_REGISTER_ARRAY;

    // 変数の大きさに合わせて アキュームレータ の使用する領域の変更
    static ERegister shrinkAccum(Token::Type* type);
    // 変数の大きさに合わせて ベースレジスタ の使用する領域を変更
    static ERegister shrinkBase(Token::Type* type);
    // 指定したレジスタの参照
    static std::string reference(ERegister reg, Token::Type* type);

    // 引数のインデックスを元に 使用するレジスタの取得
    static const ERegister argRegister(std::size_t index){return ARG_REGISTER_ARRAY.at(index);}

    Operand();
    Operand(long value);
    Operand(ERegister reg);
    Operand(std::string& str);
    Operand(std::string&& str);

    ~Operand();

    const char* operator()(){return mString.c_str();}
private:
    std::string mString;
};