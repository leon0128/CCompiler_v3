#include "file_manager.hpp"

void FileManager::write(const char* filename,
                        const char* data)
{
    std::ofstream file(filename, std::ios::out);
    if(!file.is_open())
    {
        std::cerr << "output-err: failed to open file: "
                  << filename << std::endl;
        return;
    }

    file << data;
    file.close();
}