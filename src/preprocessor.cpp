#include "preprocessor.hpp"
#include "file_manager.hpp"
#include "static_data.hpp"
#include "debugger.hpp"

Preprocessor::Preprocessor():
    mIsValid(true)
{
}

Preprocessor::~Preprocessor()
{
}

bool Preprocessor::operator()(int argc, char** argv)
{
    if(!isValidArgument(argc))
        return mIsValid;
    if(!isOpenedFile(argv))
        return mIsValid;
    if(!isDeletedComment())
        return mIsValid;
    
    Debugger::preprocessor(DATA::PREPROCESSER_DATA);
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
    if(!FileManager::read(argv[1], DATA::PREPROCESSER_DATA))
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
    while((beginPos = DATA::PREPROCESSER_DATA.find("//")) != std::string::npos)
    {
        endPos = DATA::PREPROCESSER_DATA.find("\n", beginPos);

        size_t count
            = (endPos != std::string::npos)
                ? (endPos - beginPos) : (DATA::PREPROCESSER_DATA.size() - 1);
        DATA::PREPROCESSER_DATA.replace(beginPos,
                                        count,
                                        "");
    }

    // /* *** */ コメントの置換
    while((beginPos = DATA::PREPROCESSER_DATA.find("/*")) != std::string::npos)
    {
        endPos = DATA::PREPROCESSER_DATA.find("*/", beginPos + 2);

        if(endPos != std::string::npos)
        {
            DATA::PREPROCESSER_DATA.replace(beginPos,
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