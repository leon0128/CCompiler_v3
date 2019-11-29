#include "file_manager.hpp"

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
                        const std::string& data)
{
    return write(filename, data.c_str());
}

bool FileManager::write(const char* filename,
                        const std::stringstream& data)
{
    std::string temp(data.str());
    return write(filename, temp.c_str());
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