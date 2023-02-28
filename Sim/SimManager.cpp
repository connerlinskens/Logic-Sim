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
        SDL_SetWindowTitle(_window, "Logic Sim");
        SDL_ShowCursor(1);
    }

    // Create managers
    _renderManager = std::make_unique<RenderManager>(*_renderer);
    _mouseCollisionManager = std::make_unique<MouseCollisionManager>();

    auto& chip = _chips.emplace_back("AND", 2, 1);
    chip.SetPosition({250, 100});
    _mouseCollisionManager->AddClickable(&chip);
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
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT){
            exit();
        }
        else if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_ESCAPE) { // return true for quit signal
                exit();
            }
        }
        else if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == 1){
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                auto object = _mouseCollisionManager->CheckMouseCollision(mouseX, mouseY);
                if(object){
                    std::cout << "Clicked an object!" << std::endl;
                }
            }
        }
    }
}

void SimManager::update() {
    // TODO update sim
}

void SimManager::render() {
    // Clear screen
    SDL_RenderClear(_renderer);

    for(auto& chip : _chips){
        _renderManager->RenderChip(chip.GetChipDrawData());
    }

    // Set color to draw background
    SDL_SetRenderDrawColor(_renderer, 60, 60, 60, 255);

    // Draw buffer to screen
    SDL_RenderPresent(_renderer);
}

void SimManager::exit() {
    _running = false;
}
