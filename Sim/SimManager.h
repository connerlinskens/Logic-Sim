//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_SIMMANAGER_H
#define LOGIC_SIM_SIMMANAGER_H

#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include "RenderManager.h"

class SimManager {
public:
    SimManager();
    SimManager(int windowWidth, int windowHeight, bool fullscreen);
    ~SimManager();

    void loop();
private:
    void input();
    void update();
    void render();

    void exit();
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    std::unique_ptr<RenderManager> _renderManager;

    bool _fullscreen;
    bool _running;
};


#endif //LOGIC_SIM_SIMMANAGER_H
