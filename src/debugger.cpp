#include "debugger.hpp"
#include "file_manager.hpp"
#include "token.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

const char* Debugger::PREPROCESS_FILENAME
    = "debug/preprocessor.log";
const char* Debugger::TOKENIZER_FILENAME
    = "debug/tokenizer.log";
const char* Debugger::PARSER_FILENAME
    = "debug/parser.log";

void Debugger::preprocessor(const std::string& source)
{
    FileManager::write(PREPROCESS_FILENAME,
                       source.c_str());
}

void Debugger::tokenizer(const std::vector<Token*>& tokens)
{
    using namespace boost::property_tree;

    ptree tree;

    for(std::size_t i = 0; i < tokens.size(); i++)
        tree.put(std::to_string(i), Token::KIND_NAME_MAP.at(tokens.at(i)->kind));
    
    write_json(TOKENIZER_FILENAME,
               tree);
}

// parser() で使用する関数
namespace PARSER
{
    using namespace boost::property_tree;

    ptree consume(Token* token);
    ptree conIntegral(Token* token);
    ptree conOperator(Token* token);

    ptree consume(Token* token)
    {
        ptree tree;

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

    ptree conIntegral(Token* token)
    {
        ptree tree;

        IntegralToken* intTok
            = Token::cast<IntegralToken*>(token);
        
        std::string key(Token::KIND_NAME_MAP.at(token->kind));
        tree.put(key + std::string(".value"), intTok->value);
        return tree;
    }

    ptree conOperator(Token* token)
    {
        ptree tree;

        OperatorToken* opeTok
            = Token::cast<OperatorToken*>(token);

        std::string key(Token::KIND_NAME_MAP.at(token->kind));

        tree.add_child(key + std::string(".lhs"), consume(opeTok->lhs));
        tree.add_child(key + std::string(".rhs"), consume(opeTok->rhs));
        return tree;
    }
};

void Debugger::parser(Token* parent)
{
    boost::property_tree::ptree tree
        = PARSER::consume(parent);
    
    boost::property_tree::write_json(PARSER_FILENAME, tree);
}