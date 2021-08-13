#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
namespace NajaC
{
    std::string ReadFile(std::string_view path)
    {
        std::fstream file;
        file.open(path, std::ios::in || std::ios::binary);
        if (!file.is_open())
        {
            std::cout << "failed to open file:" << path << std::endl;
            exit(1);
        }

        std::stringstream sstream;
        sstream << file.rdbuf();
        return sstream.str();
    }
}