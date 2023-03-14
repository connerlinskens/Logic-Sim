//
// Created by Conner on 3/9/2023.
//

#include <fstream>
#include <map>
#include "ChipFactory.h"
#include "../Services/FileService.h"
#include "pugixml.hpp"
#include "../SimLogic/BasicChips/ChipAND.h"
#include "../SimLogic/BasicChips/ChipOR.h"
#include "../SimLogic/BasicChips/ChipNOT.h"
#include <filesystem>
#include <iostream>

ChipData ChipFactory::PackageChip(ProgrammableChip& chip) {
    ChipData chipData {};

    // Set properties of the chip
    chipData.name = chip.Name();
    chipData.inputs = static_cast<int>(chip.Inputs().size());
    chipData.outputs = static_cast<int>(chip.Outputs().size());
    chipData.color = chip.GetColor();

    std::map<int, std::string> nodeTags {};
    // Create a local node id to know where to place wires when creating an instance of the chip
    int localNodeID = 0;
    std::map<IONode*, int> localNodes {};
    for(auto& node : chip.Inputs()){
        if(!node->Tag().empty())
            nodeTags.insert({localNodeID, node->Tag()});

        localNodes.insert({node.get(), localNodeID});
        localNodeID++;
    }
    for(auto& node : chip.Outputs()){
        if(!node->Tag().empty())
            nodeTags.insert({localNodeID, node->Tag()});

        localNodes.insert({node.get(), localNodeID});
        localNodeID++;
    }
    chipData.nodeTags = std::move(nodeTags);

    std::vector<InternalChipData> internalChips;
    for(auto& internalChip : chip.InternalChips()){
        InternalChipData internalChipData { internalChip->Name(), internalChip->Position() };
        // Give inputs a local id
        std::vector<int> inputIDs {};
        for(auto& node : internalChip->Inputs()){
            inputIDs.push_back(localNodeID);
            localNodes.insert({node.get(), localNodeID});
            localNodeID++;
        }
        internalChipData.inputIDs = std::move(inputIDs);
        // Give outputs a local id
        std::vector<int> outputIDs {};
        for(auto& node : internalChip->Outputs()){
            outputIDs.push_back(localNodeID);
            localNodes.insert({node.get(), localNodeID});
            localNodeID++;
        }
        internalChipData.outputIDs = std::move(outputIDs);

        internalChips.push_back(std::move(internalChipData));
    }
    chipData.internalChips = std::move(internalChips);

    std::vector<WireData> wires;
    for(auto& wire : chip.InternalWires()){
        auto nodeAIt = localNodes.find(&wire->NodeA());
        auto nodeBIt = localNodes.find(&wire->NodeB());
        if(nodeAIt == localNodes.end() || nodeBIt == localNodes.end()){
            throw std::runtime_error("Found no local node(s) for internal wire, while packaging chip in ChipFactory");
        }
        wires.push_back({nodeAIt->second, nodeBIt->second});
    }
    chipData.wires = std::move(wires);

    SaveChipData(chipData);
    return chipData;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
std::unique_ptr<ProgrammableChip> ChipFactory::FabricateChip(const ChipData& chipData, const std::map<std::string, ChipData>& chipRecipes) {
    auto chip {std::make_unique<ProgrammableChip>(chipData.name, chipData.inputs, chipData.outputs, chipData.color)};

    // Keep track of localNodeIDs for wire placement
    std::map<int, IONode*> localNodeIDs {};
    int localID {0};
    for(const auto & inputNode : chip->Inputs()){
        auto nodeTagIt = chipData.nodeTags.find(localID);
        if(nodeTagIt != chipData.nodeTags.end())
            inputNode->SetTag(nodeTagIt->second);

        localNodeIDs.insert({localID++, inputNode.get()});
    }
    for(const auto & outputNode : chip->Outputs()){
        auto nodeTagIt = chipData.nodeTags.find(localID);
        if(nodeTagIt != chipData.nodeTags.end())
            outputNode->SetTag(nodeTagIt->second);

        localNodeIDs.insert({localID++, outputNode.get()});
    }

    // Add internal chips
    for(auto& internalChipData : chipData.internalChips){
        auto name = internalChipData.name;
        Chip* newChip{};
        if(name == "AND"){
            newChip = &chip->AddChip<ChipAND>(internalChipData.position);
        }
        else if(name == "OR"){
            newChip = &chip->AddChip<ChipOR>(internalChipData.position);
        }
        else if(name == "NOT"){
            newChip = &chip->AddChip<ChipNOT>(internalChipData.position);
        }
        else {
            auto chipDataIt = chipRecipes.find(name);
            if(chipDataIt == chipRecipes.end())
                throw std::runtime_error("Found unknown internal chip, trying to fabricating chip: " + chipData.name);

            newChip = &chip->AddChip(std::move(FabricateChip(chipDataIt->second, chipRecipes)));
        }

        // Add nodes to map for wire placement
        if(newChip){
            auto& inputs = newChip->Inputs();
            for(int i = 0; i < inputs.size(); i++){
                localNodeIDs.insert({internalChipData.inputIDs.at(i), inputs.at(i).get()});
            }
            auto& outputs = newChip->Outputs();
            for(int o = 0; o < outputs.size(); o++){
                localNodeIDs.insert({internalChipData.outputIDs.at(o), outputs.at(o).get()});
            }
        }
    }

    // Add internal wires
    for(auto& wire : chipData.wires){
        chip->AddInternalWire(localNodeIDs.at(wire.IDNodeA), localNodeIDs.at(wire.IDNodeB));
    }

    return std::move(chip);
}
#pragma clang diagnostic pop

void ChipFactory::SaveChipData(const ChipData& chipData) {
    pugi::xml_document doc{};

    // Chip properties
    pugi::xml_node chipNode = doc.append_child("Chip");
    chipNode.append_attribute("Name") = chipData.name.c_str();
    chipNode.append_attribute("Inputs") = chipData.inputs;
    chipNode.append_attribute("Outputs") = chipData.outputs;
    pugi::xml_node colorNode = chipNode.append_child("Color");
    colorNode.append_attribute("Red") = chipData.color.red;
    colorNode.append_attribute("Green") = chipData.color.green;
    colorNode.append_attribute("Blue") = chipData.color.blue;

    pugi::xml_node nodeTagsNode = chipNode.append_child("NodeTags");
    for(auto& nodeTag : chipData.nodeTags){
        auto nodeTagNode = nodeTagsNode.append_child(nodeTag.second.c_str());
        nodeTagNode.append_attribute("ID") = nodeTag.first;
    }

    // Internal chips
    pugi::xml_node internalChipsNode = chipNode.append_child("InternalChips");
    for(auto& internalChip : chipData.internalChips){
        std::string chipName = internalChip.name;
        // Add character in front of name if it starts with a digit
        if(isdigit(chipName.at(0)) != 0){
            chipName.insert(0, 1, 'c');
        }
        // Replace all spaces in string with an underscore, xml is unable to create children with spaces
        std::replace(chipName.begin(), chipName.end(), ' ', '_');

        pugi::xml_node internalChipNode = internalChipsNode.append_child(chipName.c_str());
        internalChipNode.append_attribute("PositionX") = internalChip.position.x;
        internalChipNode.append_attribute("PositionY") = internalChip.position.y;
        pugi::xml_node inputsNode = internalChipNode.append_child("Inputs");
        for(auto& inputID : internalChip.inputIDs){
            auto inputNode = inputsNode.append_child("I");
            inputNode.append_attribute("ID") = inputID;

        }
        pugi::xml_node outputsNode = internalChipNode.append_child("Outputs");
        for(auto& outputID : internalChip.outputIDs){
            auto outputNode = outputsNode.append_child("O");
            outputNode.append_attribute("ID") = outputID;
        }
    }

    pugi::xml_node internalWiresNode = chipNode.append_child("InternalWires");
    for(auto& wire : chipData.wires){
        pugi::xml_node internalWireNode = internalWiresNode.append_child("Wire");
        internalWireNode.append_attribute("IDNodeA") = wire.IDNodeA;
        internalWireNode.append_attribute("IDNodeB") = wire.IDNodeB;
    }

    std::string path = ChipSaveDir + chipData.name + ".chip";
    doc.save_file(path.c_str());
}

void ChipFactory::LoadChipRecipes(std::map<std::string, ChipData>& chipRecipes) {
    for(auto& file : std::filesystem::directory_iterator(ChipSaveDir)){
        ChipData chip = LoadChipData(file.path().string());
        chipRecipes.insert({chip.name, std::move(chip)});
    }
}

ChipData ChipFactory::LoadChipData(const std::string& path) {
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file(path.c_str());
    if(!result){
        std::cerr << result.description() << std::endl;
        throw std::runtime_error("Unable to load file: " + path + " in ChipFactory::LoadChipData()");
    }

    // Get chip properties
    pugi::xml_node chipNode = doc.document_element();
    std::string name = chipNode.attribute("Name").as_string();
    int inputs = chipNode.attribute("Inputs").as_int();
    int outputs = chipNode.attribute("Outputs").as_int();
    pugi::xml_node colorNode = chipNode.child("Color");
    uint8_t red = colorNode.attribute("Red").as_int();
    uint8_t green = colorNode.attribute("Green").as_int();
    uint8_t blue = colorNode.attribute("Blue").as_int();

    ChipData chipData {name, inputs, outputs, {red, green, blue, 255}};

    // Retrieve node tags
    std::map<int, std::string> nodeTags;
    for(auto& nodeTag : chipNode.child("NodeTags").children()){
        nodeTags.insert({nodeTag.attribute("ID").as_int(), nodeTag.name()});
    }
    chipData.nodeTags = std::move(nodeTags);

    // Get all internal chip data
    std::vector<InternalChipData> internalChips;
    for(auto& internalChipNode : chipNode.child("InternalChips").children()){
        std::string chipName = internalChipNode.name();
        // Remove extra c char if added in front
        if(chipName.at(0) == 'c'){
            chipName = chipName.substr(1, chipName.size());
        }
        // Change all underscores back to spaces to get original names
        std::replace(chipName.begin(), chipName.end(), '_', ' ');

        int positionX = internalChipNode.attribute("PositionX").as_int();
        int positionY = internalChipNode.attribute("PositionY").as_int();
        InternalChipData internalChipData {chipName, {positionX,positionY}};

        std::vector<int> inputIDs;
        for(auto& input : internalChipNode.child("Inputs").children()){
            inputIDs.push_back(input.attribute("ID").as_int());
        }

        std::vector<int> outputIDs;
        for(auto& output : internalChipNode.child("Outputs").children()){
            outputIDs.push_back(output.attribute("ID").as_int());
        }

        internalChipData.inputIDs = std::move(inputIDs);
        internalChipData.outputIDs = std::move(outputIDs);
        internalChips.push_back(std::move(internalChipData));
    }
    chipData.internalChips = std::move(internalChips);

    // Get all wire data
    std::vector<WireData> wires;
    for(auto& wireNode : chipNode.child("InternalWires").children()){
        wires.push_back({
           wireNode.attribute("IDNodeA").as_int(),
           wireNode.attribute("IDNodeB").as_int()
        });
    }
    chipData.wires = std::move(wires);

    return chipData;
}
