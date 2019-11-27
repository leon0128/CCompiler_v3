#include "preprocessor.hpp"
#include "file_manager.hpp"

const char* Preprocessor::OUTPUT_FILENAME
    = "tmp/preprocessor.log";

Preprocessor::Preprocessor():
    mSource(),
    mIsValid(true)
{
}

Preprocessor::~Preprocessor()
{
}

bool Preprocessor::preprocess(int argc, char** argv,
                              std::string& source)
{
    if(!isValidArgument(argc))
        return mIsValid;
    if(!isOpenedFile(argv))
        return mIsValid;
    if(!isDeletedComment())
        return mIsValid;
    
    outputData();
    
    source.swap(mSource);
    return mIsValid;
}

bool Preprocessor::isValidArgument(int argc)
{
    if(argc < 2)
    {
        std::cerr << "prep-err: invalid command line argument size "
                  << "(argument size >= 2)." << std::endl;
        mIsValid = false;
    }

    return mIsValid;
}

bool Preprocessor::isOpenedFile(char** argv)
{
    if(!FileManager::read(argv[1], mSource))
    {
        std::cerr << "prep-err: cannot open file "
                  << "( " << argv[1] << " )" << std::endl;
        mIsValid = false;
    }

    return mIsValid;
}

bool Preprocessor::isDeletedComment()
{
    std::size_t beginPos = std::string::npos;
    std::size_t endPos   = std::string::npos;

    // // コメントを 置換
    while((beginPos = mSource.find("//")) != std::string::npos)
    {
        endPos = mSource.find("\n", beginPos);

        size_t count
            = (endPos != std::string::npos)
                ? (endPos - beginPos) : (mSource.size() - 1);
        mSource.replace(beginPos,
                        count,
                        "");
    }

    // /* *** */ コメントの置換
    while((beginPos = mSource.find("/*")) != std::string::npos)
    {
        endPos = mSource.find("*/", beginPos + 2);

        if(endPos != std::string::npos)
        {
            mSource.replace(beginPos,
                            endPos + 2 - beginPos,
                            " ");
        }
        else
        {
            std::cerr << "prep-err: unterminated \'/* *** */\' comment." << std::endl;
            mIsValid = false;
            break;
        }
    }

    return mIsValid;
}

void Preprocessor::outputData() const
{
    FileManager::write(OUTPUT_FILENAME,
                       mSource.c_str());
}