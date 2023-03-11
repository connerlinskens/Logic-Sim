//
// Created by Conner on 2/27/2023.
//

#include "SimManager.h"
#include "ChipFactory.h"
#include "../Services/RandomService.h"
#include "../Services/FileService.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <imgui_stdlib.h>

SimManager::SimManager() : SimManager(1920, 1080, true) {
}

SimManager::SimManager(int windowWidth, int windowHeight, bool fullscreen) :
        _window{}, _renderer{}, _fullscreen{fullscreen}, _running{true}, _mouseX{0}, _mouseY{0}, _viewedChip{nullptr},
        _editingChipProperties{false}, _propertiesInputCount{0}, _propertiesOutputCount{0}, _propertiesColorValue{} {
    // Setting up SDL and SDL_ttf
    {
        // Init sdl
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            std::cerr << "Failed to init SDL2: " << SDL_GetError() << std::endl;
        }

        // Init ttf
        if (TTF_Init() < 0) {
            std::cerr << "Failed to init SDL2_ttf: " << TTF_GetError() << std::endl;
        }

        // Create window and renderer
        if (SDL_CreateWindowAndRenderer(windowWidth, windowHeight, _fullscreen ? SDL_WINDOW_FULLSCREEN : 0, &_window,
                                        &_renderer) < 0) {
            std::cerr << "Failed to create window and/or renderer: " << SDL_GetError() << std::endl;
        }

        // Setting some window properties
        SDL_SetWindowTitle(_window, "Digital Logic Sim");
        SDL_ShowCursor(1);
    }

    // Setup ImGui
    {
        // Create context for ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO(); (void) io;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
        ImGui_ImplSDLRenderer_Init(_renderer);
    }

    // Create managers
    _renderManager = std::make_unique<RenderManager>(*_renderer, windowWidth, windowHeight);
    _mouseCollisionManager = std::make_unique<MouseCollisionManager>();
    _simControlManager = std::make_unique<SimControlManager>();

    _renderManager->AddFontResource("ShareTechMono", "res/ShareTechMono-Regular.ttf");

    // Create buttons
    _createButton = std::make_unique<Button>(Button{"CREATE", {70, 40}, [&](const std::string& name) -> void { PackageNewChip(); }});
    _propertiesButton = std::make_unique<Button>(Button("PROPERTIES", {200, 40}, [&](const std::string& name) -> void { _editingChipProperties = !_editingChipProperties; }));

    _placeButtons.emplace_back(Button{"AND", {}, [&](const std::string& name)->void{
        _simControlManager->SelectChip(ChipType::AND,{});
    }});

    _placeButtons.emplace_back(Button{"OR", {}, [&](const std::string& name)->void{
        _simControlManager->SelectChip(ChipType::OR,{});
    }});

    _placeButtons.emplace_back(Button{"NOT", {}, [&](const std::string& name)->void{
        _simControlManager->SelectChip(ChipType::NOT,{});
    }});

    UpdatePlaceButtonPositions();

    // Start with empty programmable chip
    _topLevelChip = std::make_unique<ProgrammableChip>("",2,1);
    SetViewedChip(_topLevelChip.get());

    FileService::MakeFolder(ChipSaveDir);
}

SimManager::~SimManager() {
    // Cleaning up ImGui
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Cleaning up sdl
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void SimManager::loop() {
    // TODO limit fps?
    while(_running){
        input();
        update();
        render();
    }
}

void SimManager::input() {
    // Get mouse position
    SDL_GetMouseState(&_mouseX, &_mouseY);
    
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        // Pass input to ImGui
        ImGui_ImplSDL2_ProcessEvent(&e);

        if(e.type == SDL_QUIT){
            exit();
        }
        else if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_ESCAPE) {
                if(_editingChipProperties){
                    _editingChipProperties = false;
                }
                else if(_simControlManager->PlacingWire()){
                    _simControlManager->CancelWire();
                }
                else if(_simControlManager->PlacingChip()){
                    _simControlManager->CancelChip();
                }
                else{
                    exit();
                }
            }
            else if(e.key.keysym.sym == SDLK_DELETE){
                auto overlappingWire = _mouseCollisionManager->CheckMouseWireCollision(_mouseX, _mouseY, _viewedChip->InternalWires());
                if(overlappingWire){
                    _viewedChip->RemoveWire(overlappingWire);
                }

                auto object = _mouseCollisionManager->CheckMouseAABBCollision(_mouseX, _mouseY);
                if(object) {
                    auto chip = dynamic_cast<Chip*>(object);
                    if(chip){
                        _viewedChip->RemoveChip(chip, *_mouseCollisionManager);
                        return;
                    }
                }
            }
            else{
                _simControlManager->TypeLetter(e.key.keysym.sym);
            }
        }
        else if(e.type == SDL_MOUSEBUTTONDOWN){
            std::cout << "Clicked on position " << _mouseX << ":" << _mouseY << std::endl;
            if(e.button.button == 1){
                auto object = _mouseCollisionManager->CheckMouseAABBCollision(_mouseX, _mouseY);
                if(object){
                    std::cout << "Left clicked a clickable object." << std::endl;

                    auto chip = dynamic_cast<Chip*>(object);
                    if(chip){
                        _simControlManager->AttachChipToMouse(chip);
                        return;
                    }

                    auto node = dynamic_cast<IONode*>(object);
                    if(node){
                        _simControlManager->PlaceWire(node, *_viewedChip);
                        return;
                    }

                    object->Clicked();
                }
                else if(_simControlManager->PlacingChip()){
                    _simControlManager->PlaceChip(_viewedChip, {_mouseX, _mouseY}, _chipRecipes,_mouseCollisionManager.get());
                }
            }
            else if(e.button.button == 3){
                auto object = _mouseCollisionManager->CheckMouseAABBCollision(_mouseX, _mouseY);
                if(object){
                    std::cout << "Right clicked a clickable object." << std::endl;

                    auto node = dynamic_cast<IONode*>(object);
                    if(node){
                        object->Clicked();
                        return;
                    }
                }
            }
        }
        else if(e.type == SDL_MOUSEBUTTONUP){
            if(e.button.button == 1){
                _simControlManager->ReleaseChip();
            }
        }
    }
}

void SimManager::update() {
    _simControlManager->Update(_mouseX, _mouseY);

    UpdateProperties();

    for(auto& chip : _viewedChip->InternalChips()){
        chip->Execute();
    }
}

void SimManager::render() {
    // Clear screen
    SDL_RenderClear(_renderer);

    // Render chip internal
    if(_viewedChip){
        _renderManager->RenderChipInternal(_viewedChip->GetChipDrawData());
        _renderManager->RenderIONodes(_viewedChip->Inputs());
        _renderManager->RenderIONodes(_viewedChip->Outputs());
        _renderManager->RenderWires(_viewedChip->InternalWires());

        // Render all chips
        for(auto& chip : _viewedChip->InternalChips()){
            _renderManager->RenderChip(chip->GetChipDrawData());
            _renderManager->RenderIONodes(chip->Inputs());
            _renderManager->RenderIONodes(chip->Outputs());
        }
    }

    // Render buttons
    _renderManager->RenderButton(*_createButton);
    _renderManager->RenderButton(*_propertiesButton);
    for(auto& button : _placeButtons){
        _renderManager->RenderButton(button);
    }

    // Render temp wire when placing a new wire
    if(_simControlManager->PlacingWire()){
        auto node = _simControlManager->SelectedNodeForWire();
        _renderManager->RenderLine(node.x, node.y, _mouseX, _mouseY, NodeOffStateColor);
    }
    // Render indicator when placing a new chip
    else if(_simControlManager->PlacingChip()){
        _renderManager->RenderCircle(_mouseX, _mouseY, 15, {90, 217, 17, 255});
    }

    // Render name buffer
    _renderManager->RenderText(_simControlManager->NameBuffer(), "ShareTechMono", 25, {static_cast<int>(_renderManager->WindowSize().x*0.5), 40});

    if(_editingChipProperties){
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Chip properties", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::InputInt("Inputs", &_propertiesInputCount);

        ImGui::InputInt("Outputs", &_propertiesOutputCount);

        ImGui::ColorEdit3("Color", _propertiesColorValue);

        ImGui::End();
        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    }



    // Set color to draw background
    SDL_SetRenderDrawColor(_renderer, 60, 60, 60, 255);

    // Draw buffer to screen
    SDL_RenderPresent(_renderer);
}

void SimManager::exit() {
    _running = false;
}

void SimManager::SetViewedChip(ProgrammableChip* chip) {
    if(!chip) {return;}

    // Reset current chip
    if(_viewedChip){
        _viewedChip->ResizeChipToFitName();
        _viewedChip->ResizeChipToFitNodes();
        _viewedChip->RepositionIONodes();
        _viewedChip->SetPosition({250, 250});
        _viewedChip->SetColor({static_cast<uint8_t>(_propertiesColorValue[0] * 255),
                               static_cast<uint8_t>(_propertiesColorValue[1] * 255),
                               static_cast<uint8_t>(_propertiesColorValue[2] * 255),
                               255});
    }
    _mouseCollisionManager->ClearClickables();

    // Set new viewed chip
    _viewedChip = chip;
    _viewedChip->RepositionIONodesForInternalView(_renderManager->WindowSize());
    _viewedChip->RegisterToCollisionManager(*_mouseCollisionManager, false);

    // Set properties for UI
    _propertiesInputCount = static_cast<int>(_viewedChip->Inputs().size());
    _propertiesOutputCount = static_cast<int>(_viewedChip->Outputs().size());
    _propertiesColorValue[0] = static_cast<float>(_viewedChip->GetColor().red) / 255.0f;
    _propertiesColorValue[1] = static_cast<float>(_viewedChip->GetColor().green) / 255.0f;
    _propertiesColorValue[2] = static_cast<float>(_viewedChip->GetColor().blue) / 255.0f;

    // Add all internal chips to collision manager
    for(auto& c : chip->InternalChips()){
        c->RegisterToCollisionManager(*_mouseCollisionManager);
    }

    // Add all buttons to collision manager
    for(auto& b : _placeButtons){
        _mouseCollisionManager->AddClickable(&b);
    }
    _mouseCollisionManager->AddClickable(_createButton.get());
    _mouseCollisionManager->AddClickable(_propertiesButton.get());
}

void SimManager::PackageNewChip() {
    if(_viewedChip->InternalChips().empty()) { return; }
    // Finalize properties of to package chip
    _viewedChip->SetName(_simControlManager->NameBuffer());
    _viewedChip->SetColor({static_cast<uint8_t>(_propertiesColorValue[0] * 255),
                           static_cast<uint8_t>(_propertiesColorValue[1] * 255),
                           static_cast<uint8_t>(_propertiesColorValue[2] * 255),
                           255});
    _simControlManager->ResetNameBuffer();

    // Package chip and save its data
    auto chipData = ChipFactory::PackageChip(*_viewedChip);
    auto chipDataIt = _chipRecipes.insert({chipData.name, std::move(chipData)});

    // Add button for placing
    _placeButtons.push_back(Button{chipDataIt.first->first, {},
                                      [&](const std::string& name) -> void {
        auto chipIt = _chipRecipes.find(name);
        if(chipIt != _chipRecipes.end())
            _simControlManager->SelectChip(ChipType::PROGRAMMABLE, chipIt->second);
    }});
    UpdatePlaceButtonPositions();

    // Set new parent chip
    auto newParentChip {std::make_unique<ProgrammableChip>("", 2, 1)};
    newParentChip->AddChip(std::move(_topLevelChip));
    _topLevelChip = std::move(newParentChip);
    SetViewedChip(_topLevelChip.get());
}

void SimManager::UpdateProperties() {
    if(_propertiesInputCount < 0) {_propertiesInputCount = 0;}
    int currentInputCount = static_cast<int>(_viewedChip->Inputs().size());
    if(currentInputCount != _propertiesInputCount){
        if(_propertiesInputCount > currentInputCount)
            _viewedChip->IncrementIONode(true, *_mouseCollisionManager);
        else
            _viewedChip->DecrementIONode(true, *_mouseCollisionManager);

        _viewedChip->RepositionIONodesForInternalView(_renderManager->WindowSize());
    }

    if(_propertiesOutputCount < 0) {_propertiesOutputCount = 0;}
    int currentOutputCount = static_cast<int>(_viewedChip->Outputs().size());
    if(currentOutputCount != _propertiesOutputCount){
        if(_propertiesOutputCount > currentOutputCount)
            _viewedChip->IncrementIONode(false, *_mouseCollisionManager);
        else
            _viewedChip->DecrementIONode(false, *_mouseCollisionManager);

        _viewedChip->RepositionIONodesForInternalView(_renderManager->WindowSize());
    }
}

void SimManager::UpdatePlaceButtonPositions() {
    int margin = 10;
    int startPos = 25;
    int nextPos = startPos;
    for(auto & button : _placeButtons){
        int spaceNeeded = static_cast<int>(button.AABBExtends().x*0.5);
        nextPos += spaceNeeded;
        button.SetPosition({nextPos, _renderManager->WindowSize().y - 40});
        nextPos += spaceNeeded + margin;
    }
}
