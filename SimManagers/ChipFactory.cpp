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

ChipData ChipFactory::PackageChip(ProgrammableChip& chip) {
    ChipData chipData {};

    // Set properties of the chip
    chipData.name = chip.Name();
    chipData.inputs = static_cast<int>(chip.Inputs().size());
    chipData.outputs = static_cast<int>(chip.Outputs().size());
    chipData.color = chip.GetColor();

    // Create a local node id to know where to place wires when creating an instance of the chip
    int localNodeID = 0;
    std::map<IONode*, int> localNodes {};
    for(auto& node : chip.Inputs()){
        localNodes.insert({node.get(), localNodeID});
        localNodeID++;
    }
    for(auto& node : chip.Outputs()){
        localNodes.insert({node.get(), localNodeID});
        localNodeID++;
    }

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
        localNodeIDs.insert({localID++, inputNode.get()});
    }
    for(const auto & outputNode : chip->Outputs()){
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

    // Internal chips
    pugi::xml_node internalChipsNode = chipNode.append_child("InternalChips");
    for(auto& internalChip : chipData.internalChips){
        pugi::xml_node internalChipNode = internalChipsNode.append_child(internalChip.name.c_str());
        internalChipNode.append_attribute("PositionX") = internalChip.position.x;
        internalChipNode.append_attribute("PositionY") = internalChip.position.y;
        pugi::xml_node inputsNode = internalChipNode.append_child("Inputs");
        for(auto& inputID : internalChip.inputIDs){
            inputsNode.append_child(std::to_string(inputID).c_str());
        }
        pugi::xml_node outputsNode = internalChipNode.append_child("Outputs");
        for(auto& outputID : internalChip.outputIDs){
            outputsNode.append_child(std::to_string(outputID).c_str());
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
