//
// Created by Conner on 2/27/2023.
//

#include "SimManager.h"
#include "../SimLogic/BasicChips/ChipAND.h"
#include "../SimLogic/BasicChips/ChipOR.h"
#include "../SimLogic/BasicChips/ChipNOT.h"
#include "../SimLogic/ProgrammableChip.h"
#include "ChipFactory.h"
#include "../Services/RandomService.h"
#include "../Services/FileService.h"
#include <iostream>

SimManager::SimManager() : SimManager(1920, 1080, true) {
}

SimManager::SimManager(int windowWidth, int windowHeight, bool fullscreen) :
        _window{}, _renderer{}, _fullscreen{fullscreen}, _running{true}, _mouseX{0}, _mouseY{0}, _viewedChip{nullptr} {
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

    // Create managers
    _renderManager = std::make_unique<RenderManager>(*_renderer, windowWidth, windowHeight);
    _mouseCollisionManager = std::make_unique<MouseCollisionManager>();
    _simControlManager = std::make_unique<SimControlManager>();

    _renderManager->AddFontResource("ShareTechMono", "res/ShareTechMono-Regular.ttf");

    // Create buttons
    _createButton = std::make_unique<Button>(Button{"CREATE", {70, 40}, [&]() -> void { PackageNewChip(); }});

    auto& button1 = _placeButtons.emplace_back(Button{"AND", {50, windowHeight - 40}, [&]()->void{
        _simControlManager->SelectChip(ChipType::AND);
    }});

    auto& button2 = _placeButtons.emplace_back(Button{"OR", {110, windowHeight - 40}, [&]()->void{
        _simControlManager->SelectChip(ChipType::OR);
    }});

    auto& button3 = _placeButtons.emplace_back(Button{"NOT", {170, windowHeight - 40}, [&]()->void{
        _simControlManager->SelectChip(ChipType::NOT);
    }});

    // Start with empty programmable chip
    _topLevelChip = std::make_unique<ProgrammableChip>("",2,1);
    SetViewedChip(_topLevelChip.get());

    FileService::MakeFolder(ChipSaveDir);
}

SimManager::~SimManager() {
    // Cleaning up sdl
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
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
        if(e.type == SDL_QUIT){
            exit();
        }
        else if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_ESCAPE) {
                if(_simControlManager->PlacingWire())
                    _simControlManager->CancelWire();
                else if(_simControlManager->PlacingChip()){
                    _simControlManager->CancelChip();
                }
                else
                    exit();
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
                    _simControlManager->PlaceChip(_viewedChip, {_mouseX, _mouseY}, {}, _mouseCollisionManager.get());
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
        _viewedChip->RepositionIONodes();
        _viewedChip->SetPosition({250, 250});
    }
    _mouseCollisionManager->ClearClickables();

    // Set new viewed chip
    _viewedChip = chip;
    _viewedChip->RepositionIONodesForInternalView(_renderManager->WindowSize());
    _viewedChip->RegisterToCollisionManager(*_mouseCollisionManager, false);

    // Add all internal chips to collision manager
    for(auto& c : chip->InternalChips()){
        c->RegisterToCollisionManager(*_mouseCollisionManager);
    }

    // Add all buttons to collision manager
    for(auto& b : _placeButtons){
        _mouseCollisionManager->AddClickable(&b);
    }
    _mouseCollisionManager->AddClickable(_createButton.get());
}

void SimManager::PackageNewChip() {
    if(_viewedChip->InternalChips().empty()) { return; }
    _viewedChip->SetName(_simControlManager->NameBuffer());
    _viewedChip->SetColor({static_cast<uint8_t>(RandomService::Instance()->Random(0, 255)),
                           static_cast<uint8_t>(RandomService::Instance()->Random(0, 255)),
                            static_cast<uint8_t>(RandomService::Instance()->Random(0, 255)),
                           255});
    _viewedChip->ResizeChipToFitName();
    _simControlManager->ResetNameBuffer();

    auto chipData = ChipFactory::PackageChip(*_viewedChip);
    auto newParentChip {std::make_unique<ProgrammableChip>("", 2, 1)};
    newParentChip->AddChip(std::move(_topLevelChip));
    _topLevelChip = std::move(newParentChip);
    SetViewedChip(_topLevelChip.get());
}
