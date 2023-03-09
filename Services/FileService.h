//
// Created by Conner on 3/9/2023.
//

#ifndef LOGIC_SIM_FILESERVICE_H
#define LOGIC_SIM_FILESERVICE_H

#include <string>
#include <memory>

class FileService {
public:
    static std::string LoadFile(const std::string& path);
    static void SaveFile(const std::string& path, const std::string& content);
    static void SaveFile(const std::string& path, const std::ofstream& fileStream);
    static void MakeFolder(const std::string& path);
};


#endif //LOGIC_SIM_FILESERVICE_H
