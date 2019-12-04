#include "generator.hpp"
#include "debugger.hpp"
#include "token.hpp"
#include "static_data.hpp"

const std::unordered_map<Generator::EInstruction, std::pair<const char*, std::size_t>> Generator::INSTRUCTION_MAP
    = {{MOV,   {"mov   ", 2}}, {MOVZX, {"movzx ", 2}}, {MOVSX, {"movsx ", 2}},
       {PUSH,  {"push  ", 1}}, {POP,   {"pop   ", 1}},
       {CMP,   {"cmp   ", 2}},
       {SETE,  {"sete  ", 1}}, {SETNE, {"setne ", 1}}, {SETL,  {"setl  ", 1}}, {SETLE, {"setle ", 1}}, {SETG,  {"setg  ", 1}}, {SETGE, {"setge", 1}},
       {ADD,   {"add   ", 2}}, {SUB,   {"sub   ", 2}}, {IMUL,  {"imul  ", 1}}, {IDIV,  {"idiv  ", 1}},
       {CQO,   {"cqo   ", 0}}, {CDQE,  {"cdqe  ", 0}},
       {RET,   {"ret   ", 0}}};

Generator::Generator():
    mIsValid(true)
{
}

Generator::~Generator()
{
}

bool Generator::operator()()
{
    generate();

    Debugger::generator(DATA::GENERATOR_DATA);
    return mIsValid;
}

void Generator::generate()
{
    DATA::GENERATOR_DATA << ".intel_syntax noprefix\n"
                         << ".section .text\n"
                         << ".global main\n\n";

    consume(DATA::PARSER_DATA);
}

void Generator::consume(Token* token)
{
    if(!mIsValid)
        return;

    if(token->isParent())
        conParent(token);
    else if(token->isFunction())
        conFunction(token);
    else if(token->isArithmeticOperator())
        conArithmeticOperator(token);
    else if(token->isAssignmentOperator())
        conAssignmentOperator(token);
    else if(token->isCompareOperator())
        conCompareOperator(token);
    else if(token->isReturn())
        conReturn(token);
    else if(token->isVariable())
        conVariable(token);
    else if(token->isIntegral())
        conIntegral(token);
    else
        error(token);
}

void Generator::conParent(Token* token)
{
    ParentToken* parTok
        = Token::cast<ParentToken*>(token);

    for(auto&& token : parTok->children)
        consume(token);
}

void Generator::conFunction(Token* token)
{
    FunctionToken* funTok
        = Token::cast<FunctionToken*>(token);

    DATA::GENERATOR_DATA << funTok->name << ":" << std::endl;
    instruction(PUSH, Operand::RBP);
    instruction(MOV, Operand::RBP, Operand::RSP);
    instruction(SUB, Operand::RSP, funTok->offset);

    consume(funTok->proc);

    instruction(MOV, Operand::RSP, Operand::RBP);
    instruction(POP, Operand::RBP);
    instruction(RET);
    DATA::GENERATOR_DATA << std::endl;
}

void Generator::conArithmeticOperator(Token* token)
{
    OperatorToken* opeTok
        = Token::cast<OperatorToken*>(token);

    consume(opeTok->lhs);
    instruction(PUSH, Operand::RAX);
    consume(opeTok->rhs);
    instruction(PUSH, Operand::RAX);

    instruction(POP, Operand::RBX);
    instruction(POP, Operand::RAX);

    if(opeTok->kind == Token::PLUS)
        instruction(ADD, Operand::RAX, Operand::RBX);
    else if(opeTok->kind == Token::MINUS)
        instruction(SUB, Operand::RAX, Operand::RBX);
    else if(opeTok->kind == Token::ASTERISK)
    {
        instruction(CQO);
        instruction(IMUL, Operand::RBX);
    }
    else if(opeTok->kind == Token::VIRGULE)
    {
        instruction(CQO);
        instruction(IDIV, Operand::RBX);
    }
    else if(opeTok->kind == Token::PERCENT)
    {
        instruction(CQO);
        instruction(IDIV, Operand::RBX);
        instruction(MOV, Operand::RAX, Operand::RDX);
    }
    else
        error(token);
}

void Generator::conAssignmentOperator(Token* token)
{
    OperatorToken* opeTok
        = Token::cast<OperatorToken*>(token);

    consume(opeTok->rhs);
    instruction(MOV, opeTok->lhs, Operand::shrinkAccum(opeTok->lhs));
}

void Generator::conCompareOperator(Token* token)
{
    OperatorToken* opeTok
        = Token::cast<OperatorToken*>(token);

    consume(opeTok->lhs);
    instruction(PUSH, Operand::RAX);
    consume(opeTok->rhs);
    instruction(PUSH, Operand::RAX);

    instruction(POP, Operand::RBX);
    instruction(POP, Operand::RAX);

    instruction(CMP, Operand::RAX, Operand::RBX);

    if(token->kind == Token::CMP_EQUAL)
        instruction(SETE, Operand::AL);
    else if(token->kind == Token::CMP_NOT_EQUAL)
        instruction(SETNE, Operand::AL);
    else if(token->kind == Token::CMP_LESS)
        instruction(SETL, Operand::AL);
    else if(token->kind == Token::CMP_LESS_EQUAL)
        instruction(SETLE, Operand::AL);
    else if(token->kind == Token::CMP_GREATER)
        instruction(SETG, Operand::AL);
    else if(token->kind == Token::CMP_GREATER_EQUAL)
        instruction(SETGE, Operand::AL);
    else
        error(token);
    
    instruction(MOVZX, Operand::RAX, Operand::AL);
}

void Generator::conReturn(Token* token)
{
    ReturnToken* retTok
        = Token::cast<ReturnToken*>(token);
    
    consume(retTok->expr);
    
    instruction(MOV, Operand::RSP, Operand::RBP);
    instruction(POP, Operand::RBP);
    instruction(RET);
}

void Generator::conVariable(Token* token)
{
    VariableToken* varTok
        = Token::cast<VariableToken*>(token);

    instruction(MOV, Operand::shrinkAccum(token), token);
    
    if(Token::TYPE_SIZE_MAP.at(varTok->type) == 1)
        instruction(MOVSX, Operand::RAX, Operand::AL);
    else if(Token::TYPE_SIZE_MAP.at(varTok->type) == 2)
        instruction(MOVSX, Operand::RAX, Operand::AX);
    else if(Token::TYPE_SIZE_MAP.at(varTok->type) == 4)
        instruction(CDQE);
}

void Generator::conIntegral(Token* token)
{
    IntegralToken* intTok
        = Token::cast<IntegralToken*>(token);

    instruction(MOV, Operand::RAX, intTok->value);
}

void Generator::instruction(EInstruction inst,
                            Operand ope1,
                            Operand ope2,
                            Operand ope3,
                            Operand ope4)
{
    auto pair = INSTRUCTION_MAP.at(inst);
    Operand opes[] = {ope1, ope2, ope3, ope4};

    DATA::GENERATOR_DATA << "    "
              << pair.first;

    for(std::size_t i = 0; i < pair.second; i++)
    {
        if(i != pair.second - 1)
            DATA::GENERATOR_DATA << opes[i]() << ", ";
        else
            DATA::GENERATOR_DATA << opes[i]();
    }

    DATA::GENERATOR_DATA << "\n";
}

bool Generator::error(Token* token)
{
    std::cerr << "gene-err: token processing method is undifined "
              << "( " << Token::KIND_NAME_MAP.at(token->kind) << " )."
              << std::endl;
    mIsValid = false;
    return mIsValid;
}