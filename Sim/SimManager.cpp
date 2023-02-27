//
// Created by Conner on 2/27/2023.
//

#include <iostream>
#include "SimManager.h"

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
    // TODO expand input and make it more flexible
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_ESCAPE){
                exit();
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

    // TODO draw all objects to screen

    // Set color to draw background
    SDL_SetRenderDrawColor(_renderer, 50, 50, 50, 255);

    // Draw buffer to screen
    SDL_RenderPresent(_renderer);
}

void SimManager::exit() {
    _running = false;
}
