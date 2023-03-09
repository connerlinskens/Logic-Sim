//
// Created by Conner on 3/9/2023.
//

#include <fstream>
#include <filesystem>
#include "FileService.h"

std::string FileService::LoadFile(const std::string& path) {
    std::string fileContent{};
    std::string line;
    std::ifstream fileStream {path};

    if(fileStream.is_open()){
        while(std::getline(fileStream, line))
            fileContent.append(line);
        fileStream.close();
    }
    else{
        throw std::runtime_error("Unable to read file: " + path + " in FileService");
    }
    return {};
}

void FileService::SaveFile(const std::string& path, const std::string& content) {
    std::ofstream fileStream {path, std::ios::out};
    if(fileStream.is_open()){
        fileStream << content;
        fileStream.close();
    }
    else{
        throw std::runtime_error("Unable to write to file: " + path + " in FileService");
    }
}

void FileService::SaveFile(const std::string &path, const std::ofstream& fileStream) {
    std::ofstream file {path, std::ios::out};
    if(file.is_open()){
        file << fileStream.rdbuf();
        file.close();
    }
    else{
        throw std::runtime_error("Unable to write to file: " + path + " in FileService");
    }
}

void FileService::MakeFolder(const std::string& path) {
    std::filesystem::create_directories(path);
}
