//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_RENDERMANAGER_H
#define LOGIC_SIM_RENDERMANAGER_H

#include <SDL.h>
#include "../Data/Color.h"
#include "../Data/ChipDrawData.h"

class RenderManager {
public:
    explicit RenderManager(SDL_Renderer& renderer);

    void RenderRect(int x, int y, int w, int h, Color color = {}, bool fill = false);
    void RenderCircle(int x, int y, int radius, Color color = {});
    void RenderLine(int x1, int y1, int x2, int y2, Color color = {});

    void RenderChip(const ChipDrawData& chipDrawData);
private:
    SDL_Renderer& _renderer;
};


#endif //LOGIC_SIM_RENDERMANAGER_H
