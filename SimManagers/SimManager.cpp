//
// Created by Conner on 2/27/2023.
//

#include "SimManager.h"
#include <iostream>

SimManager::SimManager() : SimManager(1920, 1080, true) {
}

SimManager::SimManager(int windowWidth, int windowHeight, bool fullscreen) :
_window{}, _renderer{}, _fullscreen{fullscreen}, _running{true} {
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
        SDL_SetWindowTitle(_window, "Logic SimManagers");
        SDL_ShowCursor(1);
    }

    // Create managers
    _renderManager = std::make_unique<RenderManager>(*_renderer);
    _mouseCollisionManager = std::make_unique<MouseCollisionManager>();
    _simControlManager = std::make_unique<SimControlManager>();

    auto& chip = _chips.emplace_back(std::make_unique<Chip>("AND", 0, 5));
    chip->SetPosition({250, 100});
    chip->RegisterToCollisionManager(*_mouseCollisionManager);

    auto& chip2 = _chips.emplace_back(std::make_unique<Chip>("AND", 4, 1));
    chip2->SetPosition({500, 100});
    chip2->RegisterToCollisionManager(*_mouseCollisionManager);
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
                else
                    exit();
            }
        }
        else if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == 1){
                auto object = _mouseCollisionManager->CheckMouseCollision(_mouseX, _mouseY);
                if(object){
                    std::cout << "Left clicked a clickable object." << std::endl;

                    auto chip = dynamic_cast<Chip*>(object);
                    if(chip){
                        _simControlManager->AttachChipToMouse(chip);
                        return;
                    }

                    auto node = dynamic_cast<IONode*>(object);
                    if(node){
                        _simControlManager->PlaceWire(node);
                        return;
                    }

                    object->Clicked();
                }
            }
            else if(e.button.button == 3){
                auto object = _mouseCollisionManager->CheckMouseCollision(_mouseX, _mouseY);
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
}

void SimManager::render() {
    // Clear screen
    SDL_RenderClear(_renderer);

    // Render all chips to screen
    for(auto& chip : _chips){
        _renderManager->RenderChip(chip->GetChipDrawData());
    }

    // TODO if placing a wire, draw a temp wire
    if(_simControlManager->PlacingWire()){
        auto node = _simControlManager->SelectedNodeForWire();
        _renderManager->RenderLine(node.x, node.y, _mouseX, _mouseY, NodeOffStateColor);
    }

    // Set color to draw background
    SDL_SetRenderDrawColor(_renderer, 60, 60, 60, 255);

    // Draw buffer to screen
    SDL_RenderPresent(_renderer);
}

void SimManager::exit() {
    _running = false;
}
