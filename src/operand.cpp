#include "operand.hpp"

const std::unordered_map<Operand::ERegister, const char*> Operand::REGISTER_NAME_MAP
    = {{Operand::RAX,     "rax"}, {Operand::RBX,     "rbx"}, {Operand::RCX,     "rcx"}, {Operand::RDX,     "rdx"},
       {Operand::RSI,     "rsi"}, {Operand::RDI,     "rdi"}, {Operand::RSP,     "rsp"}, {Operand::RBP,     "rbp"},
       {Operand::R8,       "r8"}, {Operand::R9,       "r9"}, {Operand::R10,     "r10"}, {Operand::R11,     "r11"},
       {Operand::R12,     "r12"}, {Operand::R13,     "r13"}, {Operand::R14,     "r14"}, {Operand::R15,     "r15"},
       {Operand::EAX,     "eax"}, {Operand::EBX,     "ebx"}, {Operand::ECX,     "ecx"}, {Operand::EDX,     "edx"},
       {Operand::ESI,     "esi"}, {Operand::EDI,     "edi"}, {Operand::ESP,     "esp"}, {Operand::EBP,     "ebp"},
       {Operand::R8D,     "r8d"}, {Operand::R9D,     "r9d"}, {Operand::R10D,   "r10d"}, {Operand::R11D,   "r11d"},
       {Operand::R12D,   "r12d"}, {Operand::R13D,   "r13d"}, {Operand::R14D,   "r14d"}, {Operand::R15D,   "r15d"},
       {Operand::AX,       "ax"}, {Operand::BX,       "bx"}, {Operand::CX,       "cx"}, {Operand::DX,       "dx"},
       {Operand::SI,       "si"}, {Operand::DI,       "di"}, {Operand::SP,       "sp"}, {Operand::BP,       "bp"},
       {Operand::R8W,     "r8w"}, {Operand::R9W,     "r9w"}, {Operand::R10W,   "r10w"}, {Operand::R11W,   "r11w"},
       {Operand::R12W,   "r12w"}, {Operand::R13W,   "r13w"}, {Operand::R14W,   "r14w"}, {Operand::R15W,   "r15w"},
       {Operand::AH,       "ah"}, {Operand::BH,       "bh"}, {Operand::CH,       "ch"}, {Operand::DH,       "dh"},
       {Operand::AL,       "al"}, {Operand::BL,       "bl"}, {Operand::CL,       "cl"}, {Operand::DL,       "dl"},
       {Operand::SIL,     "sil"}, {Operand::DIL,     "dil"}, {Operand::SPL,     "spl"}, {Operand::BPL,     "bpl"},
       {Operand::R8L,     "r8l"}, {Operand::R9L,     "r9l"}, {Operand::R10L,   "r10l"}, {Operand::R11L,   "r11l"},
       {Operand::R12L,   "r12l"}, {Operand::R13L,   "r13l"}, {Operand::R14L,   "r14l"}, {Operand::R15L,   "r15l"},
       {Operand::R0,       "r0"}, {Operand::R1,       "r1"}, {Operand::R2,       "r2"}, {Operand::R3,       "r3"},
       {Operand::R4,       "r4"}, {Operand::R5,       "r5"}, {Operand::R6,       "r6"}, {Operand::R7,       "r7"},
       {Operand::MM0,     "mm0"}, {Operand::MM1,     "mm1"}, {Operand::MM2,     "mm2"}, {Operand::MM3,     "mm3"},
       {Operand::MM4,     "mm4"}, {Operand::MM5,     "mm5"}, {Operand::MM6,     "mm6"}, {Operand::MM7,     "mm7"},
       {Operand::XMM0,   "xmm0"}, {Operand::XMM1,   "xmm1"}, {Operand::XMM2,   "xmm2"}, {Operand::XMM3,   "xmm3"},
       {Operand::XMM4,   "xmm4"}, {Operand::XMM5,   "xmm5"}, {Operand::XMM6,   "xmm6"}, {Operand::XMM7,   "xmm7"},
       {Operand::XMM8,   "xmm8"}, {Operand::XMM9,   "xmm9"}, {Operand::XMM10, "xmm10"}, {Operand::XMM11, "xmm11"},
       {Operand::XMM12, "xmm12"}, {Operand::XMM13, "xmm13"}, {Operand::XMM14, "xmm14"}, {Operand::XMM15, "xmm15"},
       {Operand::YMM0,   "ymm1"}, {Operand::YMM1,   "ymm1"}, {Operand::YMM2,   "ymm2"}, {Operand::YMM3,   "ymm3"},
       {Operand::YMM4,   "ymm4"}, {Operand::YMM5,   "ymm5"}, {Operand::YMM6,   "ymm6"}, {Operand::YMM7,   "ymm7"},
       {Operand::YMM8,   "ymm8"}, {Operand::YMM9,   "ymm9"}, {Operand::YMM10, "ymm10"}, {Operand::YMM11, "ymm11"},
       {Operand::YMM12, "ymm12"}, {Operand::YMM13, "ymm13"}, {Operand::YMM14, "ymm14"}, {Operand::YMM15, "ymm15"}};

Operand::Operand():
    mString()
{
}

Operand::Operand(long value):
    mString(std::to_string(value))
{
}

Operand::Operand(ERegister reg):
    mString(REGISTER_NAME_MAP.at(reg))
{
}

Operand::Operand(ERegister reg, long offset):
    mString()
{
    std::stringstream stream;
    stream << "QWORD PTR "
           << "[" << REGISTER_NAME_MAP.at(reg)
           << ((offset >= 0) ? " + " : " - ")
           << std::abs(offset) << "]";
    mString = stream.str();
}

Operand::~Operand()
{
}