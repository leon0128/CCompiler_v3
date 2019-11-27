#include "compiler.hpp"
#include "preprocessor.hpp"
#include "tokenizer.hpp"
#include "token.hpp"

Compiler::Compiler():
    mPreprocessor(nullptr),
    mTokenizer(nullptr),
    mSource(),
    mTokens(),
    mIsValid(true)
{
    mPreprocessor = new Preprocessor();
    mTokenizer    = new Tokenizer();
}

Compiler::~Compiler()
{
    delete mTokenizer;
    delete mPreprocessor;

    Token::destroy();
}

bool Compiler::operator()(int argc, char** argv)
{
    if(!mPreprocessor->preprocess(argc, argv, mSource))
        return error("failed to preprocess.");
    if(!mTokenizer->tokenize(mSource, mTokens))
        return error("failed to tokenize.");

    return mIsValid;
}

bool Compiler::error(const char* message)
{
    std::cerr << "comp-err: "
              << message << std::endl;

    mIsValid = false;
    return mIsValid;
}