#include "compiler.hpp"
#include "preprocessor.hpp"

Compiler::Compiler():
    mPreprocessor(nullptr)
{
    mPreprocessor = new Preprocessor();
}

Compiler::~Compiler()
{
    delete mPreprocessor;
}

bool Compiler::operator()(int argc, char** argv)
{
    return true;
}