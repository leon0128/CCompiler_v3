#include "file_manager.hpp"

void FileManager::write(const char* filename,
                        const char* data)
{
    std::ofstream file(filename, std::ios::out);
    if(!file.is_open())
    {
        openError(filename);
        return;
    }

    file << data;
    file.close();
}

void FileManager::read(const char* filename,
                       std::string& data)
{
    std::ifstream file(filename, std::ios::in);
    if(!file.is_open())
    {
        openError(filename);
        return;
    }

    std::stringstream stream;
    stream << file.rdbuf();

    data = stream.str();
    file.close();
}

void FileManager::openError(const char* filename)
{
    std::cerr << "file-err: failed to open file "
              << "( " << filename << " )." << std::endl;  
}