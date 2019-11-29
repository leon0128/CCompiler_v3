#pragma once

#include <string>
#include <sstream>
#include <iostream>

class ParserDebugger
{
public:
    ParserDebugger();

    bool debug(const char* filename,
               class Token* parent);

private:
    void consume(class Token* token);
    void conOperator(class Token* token);
    void conIntegral(class Token* token);

    bool error(class Token* token);

    std::stringstream mStream;
    std::size_t mIndent;
};