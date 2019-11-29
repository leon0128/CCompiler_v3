#include "file_manager.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

bool FileManager::write(const char* filename,
                        const char* data)
{
    std::ofstream file(filename, std::ios::out);
    if(!file.is_open())
        return openError(filename);

    file << data;
    file.close();

    return true;
}

bool FileManager::write(const char* filename,
                        void* pTree)
{
    boost::property_tree::ptree* tree 
        = static_cast<boost::property_tree::ptree*>(pTree);

    try
    {
        boost::property_tree::write_json(filename,
                                         *tree);
        return true;
    }
    catch(...)
    {
        openError(filename);
        return false;
    }
}

bool FileManager::read(const char* filename,
                       std::string& data)
{
    std::ifstream file(filename, std::ios::in);
    if(!file.is_open())
        return openError(filename);

    std::stringstream stream;
    stream << file.rdbuf();

    data = stream.str();
    file.close();
    
    return true;
}

bool FileManager::openError(const char* filename)
{
    std::cerr << "file-err: failed to open file "
              << "( " << filename << " )." << std::endl; 
    return false; 
}