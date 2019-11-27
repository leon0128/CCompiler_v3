#include "compiler.hpp"
#include "preprocessor.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "token.hpp"

Compiler::Compiler():
    mPreprocessor(nullptr),
    mTokenizer(nullptr),
    mParser(nullptr),
    mSource(),
    mTokens(),
    mParent(nullptr),
    mIsValid(true)
{
    mPreprocessor = new Preprocessor();
    mTokenizer    = new Tokenizer();
    mParser       = new Parser();
}

Compiler::~Compiler()
{
    delete mParser;
    delete mTokenizer;
    delete mPreprocessor;

    Token::destroy();
}

bool Compiler::operator()(int argc, char** argv)
{
    if(!(*mPreprocessor)(argc, argv, mSource))
        return error("failed to preprocess.");
    if(!(*mTokenizer)(mSource, mTokens))
        return error("failed to tokenize.");
    if(!(*mParser)(mTokens, mParent))
        return error("failed to parse.");

    return mIsValid;
}

bool Compiler::error(const char* message)
{
    std::cerr << "comp-err: "
              << message << std::endl;

    mIsValid = false;
    return mIsValid;
}