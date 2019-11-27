#include "compiler.hpp"
#include "preprocessor.hpp"

Compiler::Compiler():
    mPreprocessor(nullptr),
    mSource()
{
    mPreprocessor = new Preprocessor();
}

Compiler::~Compiler()
{
    delete mPreprocessor;
}

bool Compiler::operator()(int argc, char** argv)
{
    if(!mPreprocessor->preprocess(argc, argv, mSource))
    {
        std::cerr << "err: failed to preprocess." << std::endl;
        return false;
    }

    return true;
}