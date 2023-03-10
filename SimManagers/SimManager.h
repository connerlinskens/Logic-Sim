//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_SIMMANAGER_H
#define LOGIC_SIM_SIMMANAGER_H

#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include "RenderManager.h"
#include "../SimLogic/Chip.h"
#include "MouseCollisionManager.h"
#include "SimControlManager.h"
#include "../SimLogic/ProgrammableChip.h"
#include "../Data/ChipData.h"

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

    void SetViewedChip(ProgrammableChip* chip);
    void PackageNewChip();
    void UpdateProperties();
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    std::unique_ptr<RenderManager> _renderManager;
    std::unique_ptr<MouseCollisionManager> _mouseCollisionManager;
    std::unique_ptr<SimControlManager> _simControlManager;

    bool _fullscreen;
    bool _running;

    int _mouseX, _mouseY;

    std::unique_ptr<ProgrammableChip> _topLevelChip;
    ProgrammableChip* _viewedChip;

    std::vector<Button> _placeButtons;
    std::unique_ptr<Button> _createButton;

    std::unique_ptr<Button> _propertiesButton;
    bool _editingChipProperties;
    int _propertiesInputCount, _propertiesOutputCount;
    float _propertiesColorValue[3];

    std::vector<ChipData> _chipDataList;
};


#endif //LOGIC_SIM_SIMMANAGER_H
