//
// Created by Conner on 3/9/2023.
//

#include <fstream>
#include "ChipFactory.h"
#include "../Services/FileService.h"
#include "pugixml.hpp"

std::string ChipFactory::PackageChip(ProgrammableChip& chip) {
    pugi::xml_document doc{};

    // Chip properties
    pugi::xml_node chipNode = doc.append_child("Chip");
    chipNode.append_attribute("Name") = chip.Name().c_str();
    chipNode.append_attribute("Inputs") = chip.Inputs().size();
    chipNode.append_attribute("Outputs") = chip.Outputs().size();
    pugi::xml_node colorNode = chipNode.append_child("Color");
    Color color = chip.GetColor();
    colorNode.append_attribute("Red") = color.red;
    colorNode.append_attribute("Green") = color.green;
    colorNode.append_attribute("Blue") = color.blue;
    // Internal chips
    pugi::xml_node internalChipsNode = chipNode.append_child("InternalChips");
    for(auto& internalChip : chip.InternalChips()){
        pugi::xml_node internalChipNode = internalChipsNode.append_child(internalChip->Name().c_str());
        internalChipNode.append_attribute("PositionX") = internalChip->Position().x;
        internalChipNode.append_attribute("PositionY") = internalChip->Position().y;
    }

    std::string path = ChipSaveDir + chip.Name() + ".chip";
    doc.save_file(path.c_str());
//    std::ofstream stream {};
//    doc.save(stream);
//    FileService::SaveFile(path, stream);
//    stream.close();
    std::string chipData {};
    return chipData;
}

std::unique_ptr<ProgrammableChip> ChipFactory::FabricateChip(const std::string &chipData) {
    auto chip {std::make_unique<ProgrammableChip>("TEST", 1, 1)};
    return std::move(chip);
}
