//
// Created by Conner on 3/9/2023.
//

#ifndef LOGIC_SIM_XMLSERVICE_H
#define LOGIC_SIM_XMLSERVICE_H

#include <string>
#include <memory>

class XMLService {
public:
    static std::string LoadFile(const std::string& path);
    static void SaveFile(const std::string& path, const std::string& content);
};


#endif //LOGIC_SIM_XMLSERVICE_H
