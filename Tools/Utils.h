#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
inline std::string ReadFile(std::string path)
{
    std::fstream file;
    file.open(path.data(), std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "failed to open file:" << path << std::endl;
        exit(1);
    }

    std::stringstream sstream;
    sstream << file.rdbuf();
    return sstream.str();
}