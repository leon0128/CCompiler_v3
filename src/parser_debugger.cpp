#include "parser_debugger.hpp"
#include "token.hpp"

void ParserDebugger::debug(Token* parent,
                                  const char* filename)
{
    boost::property_tree::ptree tree
        = consume(parent);

    boost::property_tree::write_json(filename, tree);
}

boost::property_tree::ptree ParserDebugger::consume(Token* token)
{
    boost::property_tree::ptree tree;

    if(token->isIntegral())
        tree = conIntegral(token);
    else if(token->isOperator())
        tree = conOperator(token);
    else
    {
        std::cerr << "debu-err: unimplemented type "
                    << "( " << Token::KIND_NAME_MAP.at(token->kind)
                    << " )." << std::endl; 
    }

    return tree;
}

boost::property_tree::ptree ParserDebugger::conIntegral(Token* token)
{
    boost::property_tree::ptree tree;

    IntegralToken* intTok
        = Token::cast<IntegralToken*>(token);
    
    std::string key(Token::KIND_NAME_MAP.at(token->kind));
    tree.put(key + std::string(".value"), intTok->value);

    return tree;
}

boost::property_tree::ptree ParserDebugger::conOperator(Token* token)
{
    boost::property_tree::ptree tree;

    OperatorToken* opeTok
        = Token::cast<OperatorToken*>(token);

    std::string key(Token::KIND_NAME_MAP.at(token->kind));

    tree.add_child(key + std::string(".lhs"), consume(opeTok->lhs));
    tree.add_child(key + std::string(".rhs"), consume(opeTok->rhs));

    return tree;
}