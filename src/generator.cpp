#include "generator.hpp"
#include "debugger.hpp"
#include "token.hpp"

const std::unordered_map<Generator::EInstruction, std::pair<const char*, std::size_t>> Generator::INSTRUCTION_MAP
    = {{MOV,  {"mov  ", 2}},
       {PUSH, {"push ", 1}}, {POP,  {"pop  ", 1}},
       {ADD,  {"add  ", 2}}, {SUB,  {"sub  ", 2}}, {IMUL, {"imul ", 1}}, {IDIV, {"idiv ", 1}},
       {CQO,  {"cqo  ", 0}},
       {RET,  {"ret  ", 0}}};

Generator::Generator():
    mParent(nullptr),
    mAssembly(),
    mIsValid(true)
{
}

Generator::~Generator()
{
}

bool Generator::operator()(Token* parent,
                           std::stringstream& assembly)
{
    mParent = parent;

    generate();

    Debugger::generator(mAssembly);

    assembly.swap(mAssembly);
    return mIsValid;
}

void Generator::generate()
{
    mAssembly << ".intel_syntax noprefix\n"
              << ".global main\n\n"
              << "main:\n";

    instruction(PUSH, Operand::RBP);
    instruction(MOV, Operand::RBP, Operand::RSP);

    consume(mParent);

    instruction(MOV, Operand::RSP, Operand::RBP);
    instruction(POP, Operand::RBP);
    instruction(RET);
}

void Generator::consume(Token* token)
{
    if(!mIsValid)
        return;

    if(token->isParent())
        conParent(token);
    else if(token->isOperator())
        conOperator(token);
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

void Generator::conOperator(Token* token)
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

    mAssembly << "    "
              << pair.first;

    for(std::size_t i = 0; i < pair.second; i++)
    {
        if(i != pair.second - 1)
            mAssembly << opes[i]() << ", ";
        else
            mAssembly << opes[i]();
    }

    mAssembly << "\n";
}

bool Generator::error(Token* token)
{
    std::cerr << "gene-err: token processing method is undifined "
              << "( " << Token::KIND_NAME_MAP.at(token->kind) << " )."
              << std::endl;
    mIsValid = false;
    return mIsValid;
}