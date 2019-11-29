#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class FileManager
{
public:
    FileManager() = delete;

    // 書き込み
    static bool write(const char* filename,
                      const char* data);
    static bool write(const char* filename,
                      const std::string& data);
    static bool write(const char* filename,
                      const std::stringstream& data);

    // 読み込み
    static bool read(const char* filename,
                     std::string& data);

private:
    static bool openError(const char* filename);
};