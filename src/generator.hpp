#pragma once

#include <string>

class Generator
{
public:
    Generator();
    ~Generator();

    bool operator()(class Token* parent,
                    std::string& assembly);
    
private:
    void generate();

    void consume(class Token* token);
    void conOperator(class Token* token);
    void conIntegral(class Token* token);

    class Token* mParent;
    std::string mAssembly;
    bool mIsValid;
};