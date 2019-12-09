#include "generator.hpp"
#include "debugger.hpp"
#include "token.hpp"
#include "static_data.hpp"

const std::unordered_map<Generator::EInstruction, std::pair<const char*, std::size_t>> Generator::INSTRUCTION_MAP
    = {{MOV,   {"mov   ", 2}}, {MOVZX, {"movzx ", 2}}, {MOVSX, {"movsx ", 2}},
       {PUSH,  {"push  ", 1}}, {POP,   {"pop   ", 1}},
       {CMP,   {"cmp   ", 2}}, {JMP,   {"jmp   ", 1}}, {JNE,   {"jne   ", 1}},
       {SETE,  {"sete  ", 1}}, {SETNE, {"setne ", 1}}, {SETL,  {"setl  ", 1}}, {SETLE, {"setle ", 1}}, {SETG,  {"setg  ", 1}}, {SETGE, {"setge", 1}},
       {ADD,   {"add   ", 2}}, {SUB,   {"sub   ", 2}}, {IMUL,  {"imul  ", 1}}, {IDIV,  {"idiv  ", 1}},
       {CQO,   {"cqo   ", 0}}, {CDQE,  {"cdqe  ", 0}},
       {CALL,  {"call  ", 1}}, {RET,   {"ret   ", 0}}};

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

    Debugger::generator(DATA::GENERATOR_DATA());
    return mIsValid;
}

void Generator::generate()
{
    DATA::GENERATOR_DATA() << ".intel_syntax noprefix\n"
                           << ".section .text\n"
                           << ".global ";

    for(std::size_t i = 0; i < DATA::FUNCTION_TRAITS().size(); i++)
    {
        if(i != DATA::FUNCTION_TRAITS().size() - 1)
            DATA::GENERATOR_DATA() << DATA::FUNCTION_TRAITS().at(i).name << ", ";
        else
            DATA::GENERATOR_DATA() << DATA::FUNCTION_TRAITS().at(i).name << std::endl;
    }
    DATA::GENERATOR_DATA() << std::endl;

    consume(DATA::PARSER_DATA());
}

void Generator::consume(Token* token)
{
    if(!mIsValid)
        return;

    if(!token)
        return;

    if(token->isParent())
        conParent(token);
    else if(token->isFunction())
        conFunction(token);
    else if(token->isCall())
        conCall(token);
    else if(token->isWhile())
        conWhile(token);
    else if(token->isFor())
        conFor(token);
    else if(token->isIf())
        conIf(token);
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

    DATA::GENERATOR_DATA() << funTok->name << ":" << std::endl;
    instruction(PUSH, Operand::RBP);
    instruction(MOV, Operand::RBP, Operand::RSP);
    instruction(SUB, Operand::RSP, funTok->offset);

    consume(funTok->proc);

    instruction(MOV, Operand::RSP, Operand::RBP);
    instruction(POP, Operand::RBP);
    instruction(RET);
    DATA::GENERATOR_DATA() << std::endl;
}

void Generator::conCall(Token* token)
{
    CallToken* calTok
        = Token::cast<CallToken*>(token);

    std::size_t callerArgSize = 0;
    for(auto&& e : DATA::FUNCTION_TRAITS())
    {
        if(e.name == calTok->name)
        {
            callerArgSize = e.argsType.size();
            break;
        }
    }

    for(std::size_t i = callerArgSize; i > 0; i--)
        instruction(PUSH, Operand::argRegister(i - 1));

    for(auto&& e : calTok->args)
    {
        consume(e);
        instruction(PUSH, Operand::RAX);
    }
    for(std::size_t i = calTok->args.size(); i > 0; i--)
        instruction(POP, Operand::argRegister(i - 1));
    
    instruction(CALL, calTok->name);
    
    for(std::size_t i = 0; i < callerArgSize; i++)
        instruction(POP, Operand::argRegister(i));
}

void Generator::conWhile(Token* token)
{
    WhileToken* whiTok
        = Token::cast<WhileToken*>(token);
    
    std::string label(".LW");
    label += std::to_string(whiTok->label);
    label.push_back('C');

    instruction(JMP, label);
    label.back() = 'P';
    DATA::GENERATOR_DATA() << label << ":" << std::endl;
    consume(whiTok->proc);
    label.back() = 'C';
    DATA::GENERATOR_DATA() << label << ":" << std::endl;
    consume(whiTok->cmp);
    instruction(CMP, Operand::RAX, 0L);
    label.back() = 'P';
    instruction(JNE, label);
}

void Generator::conFor(Token* token)
{
    ForToken* forTok
        = Token::cast<ForToken*>(token);

    std::string label(".LF");
    label += std::to_string(forTok->label);
    label.push_back('C');

    consume(forTok->init);
    instruction(JMP, label);
    label.back() = 'P';
    DATA::GENERATOR_DATA() << label << ":" << std::endl;
    consume(forTok->proc);
    consume(forTok->rate);
    label.back() = 'C';
    DATA::GENERATOR_DATA() << label << ":" << std::endl;
    consume(forTok->cmp);
    instruction(CMP, Operand::RAX, 0L);
    label.back() = 'P';
    instruction(JNE, label);
}

void Generator::conIf(Token* token)
{
    IfToken* ifTok
        = Token::cast<IfToken*>(token);

    std::string label(".LI");
    label += std::to_string(ifTok->label);
    for(std::size_t i = 0; i < ifTok->children.size(); i++)
    {
        if(ifTok->children.at(i).cmp)
        {
            consume(ifTok->children.at(i).cmp);
            instruction(CMP, Operand::RAX, 0L);
        }
        DATA::GENERATOR_DATA() << label << "_" << i << ":" << std::endl;
        consume(ifTok->children.at(i).proc);
        label.push_back('E');
        instruction(JMP, label);
        label.pop_back();
    }

    DATA::GENERATOR_DATA() << label << "E:" << std::endl;
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
    VariableToken* varTok
        = Token::cast<VariableToken*>(opeTok->lhs);

    consume(opeTok->rhs);

    if(varTok->isArg)
        instruction(MOV, Operand::argRegister(varTok->offset), Operand::RAX);
    else
    {
        instruction(MOV, Operand::RBX, Operand::RAX);
        instruction(MOV, Operand::RAX, Operand::RBP);
        instruction(SUB, Operand::RAX, varTok->offset);
        instruction(MOV, Operand::reference(Operand::RAX, varTok->type->type), Operand::shrinkBase(varTok->type->type));
        instruction(MOV, Operand::shrinkAccum(varTok->type->type), Operand::reference(Operand::RAX, varTok->type->type)); 
        exAccum(varTok->type->type);    
    }
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

    if(varTok->isArg)
        instruction(MOV, Operand::RAX, Operand::argRegister(varTok->offset))
    else
    {
        instruction(MOV, Operand::RAX, Operand::RBP);
        instruction(SUB, Operand::RAX, varTok->offset);
        instruction(MOV, Operand::RAX, Operand::reference(Operand::RAX, varTok->type->type));
        exAccum(varTok->type->type);    
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

    DATA::GENERATOR_DATA() << "    "
              << pair.first;

    for(std::size_t i = 0; i < pair.second; i++)
    {
        if(i != pair.second - 1)
            DATA::GENERATOR_DATA() << opes[i]() << ", ";
        else
            DATA::GENERATOR_DATA() << opes[i]();
    }

    DATA::GENERATOR_DATA() << "\n";
}

bool Generator::error(Token* token)
{
    std::cerr << "gene-err: token processing method is undifined "
              << "( " << Token::KIND_NAME_MAP.at(token->kind) << " )."
              << std::endl;
    mIsValid = false;
    return mIsValid;
}

void Generator::exAccum(Token::EType type)
{
    if(Token::TYPE_SIZE_MAP.at(varTok->type->type) == 1)
        instruction(MOVSX, Operand::RAX, Operand::AL);
    else if(Token::TYPE_SIZE_MAP.at(varTok->type->type) == 2)
        instruction(MOVSX, Operand::RAX, Operand::AX);
    else if(Token::TYPE_SIZE_MAP.at(varTok->type->type) == 4)
        instruction(CDQE);
}