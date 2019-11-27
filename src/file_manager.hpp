#pragma once

#include <iostream>
#include <fstream>
#include <string>

class FileManager
{
public:
    FileManager() = delete;

    static void write(const char* filename,
                      const char* data);
};