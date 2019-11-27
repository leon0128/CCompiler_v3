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
    static void write(const char* filename,
                      const char* data);

    // 読み込み
    static void read(const char* filename,
                     std::string& data);

private:
    static void openError(const char* filename);
};