#pragma once

#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class ParserDebugger
{
public:
    ParserDebugger() = delete;

    static void debug(class Token* parent,
                      const char* filename);

private:
    static boost::property_tree::ptree consume(Token* token);
    static boost::property_tree::ptree conOperator(Token* token);
    static boost::property_tree::ptree conIntegral(Token* token);
};