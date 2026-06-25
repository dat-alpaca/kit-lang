#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace kit
{
    inline std::string file_read_contents(const std::filesystem::path& filepath)
    {
        std::fstream file(filepath);
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
}