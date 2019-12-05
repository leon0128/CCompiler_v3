#include "compiler.hpp"
#include "preprocessor.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "generator.hpp"
#include "token.hpp"
#include "file_manager.hpp"
#include "static_data.hpp"

const char* Compiler::RESULT_FILENAME
    = "as.s";

Compiler::Compiler():
    mPreprocessor(nullptr),
    mTokenizer(nullptr),
    mParser(nullptr),
    mGenerator(nullptr),
    mIsValid(true)
{
    mPreprocessor = new Preprocessor();
    mTokenizer    = new Tokenizer();
    mParser       = new Parser();
    mGenerator    = new Generator();
}

Compiler::~Compiler()
{
    delete mGenerator;
    delete mParser;
    delete mTokenizer;
    delete mPreprocessor;

    Token::destroy();
}

bool Compiler::operator()(int argc, char** argv)
{
    if(!(*mPreprocessor)(argc, argv))
        return error("failed to preprocess.");
    if(!(*mTokenizer)())
        return error("failed to tokenize.");
    if(!(*mParser)())
        return error("failed to parse.");
    if(!(*mGenerator)())
        return error("failed to generate.");

    FileManager::write(RESULT_FILENAME,
                       DATA::GENERATOR_DATA());

    return mIsValid;
}

bool Compiler::error(const char* message)
{
    std::cerr << "comp-err: "
              << message << std::endl;

    mIsValid = false;
    return mIsValid;
}