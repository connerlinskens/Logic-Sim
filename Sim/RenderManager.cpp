//
// Created by Conner on 2/27/2023.
//

#include "RenderManager.h"
#include <cmath>

RenderManager::RenderManager(SDL_Renderer& renderer) : _renderer{renderer} {
}

void RenderManager::RenderRect(int x, int y, int w, int h, Color color, bool fill) {
    int offsetX, offsetY;
    offsetX =  floor(w / 2.0);
    offsetY = floor(h / 2.0);

    SDL_SetRenderDrawColor(&_renderer, color.red, color.green, color.blue, color.alpha);

    SDL_Rect rect {x-offsetX,y-offsetY,w,h};
    if(fill)
        SDL_RenderFillRect(&_renderer, &rect);
    else
        SDL_RenderDrawRect(&_renderer, &rect);
}

/// Credit: https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c
void RenderManager::RenderCircle(int x, int y, int radius, Color color) {
    int offsetX, offsetY, d;

    radius = floor(radius / 2.0);

    offsetX = 0;
    offsetY = radius;
    d = radius - 1;

    SDL_SetRenderDrawColor(&_renderer, color.red, color.green, color.blue, color.alpha);
    
    while (offsetY >= offsetX) {

        SDL_RenderDrawLine(&_renderer, x - offsetY, y + offsetX,
                           x + offsetY, y + offsetX);
        SDL_RenderDrawLine(&_renderer, x - offsetX, y + offsetY,
                                     x + offsetX, y + offsetY);
        SDL_RenderDrawLine(&_renderer, x - offsetX, y - offsetY,
                                     x + offsetX, y - offsetY);
        SDL_RenderDrawLine(&_renderer, x - offsetY, y - offsetX,
                           x + offsetY, y - offsetX);

        if (d >= 2 * offsetX) {
            d -= 2 * offsetX + 1;
            offsetX += 1;
        } else if (d < 2 * (radius - offsetY)) {
            d += 2 * offsetY - 1;
            offsetY -= 1;
        } else {
            d += 2 * (offsetY - offsetX - 1);
            offsetY -= 1;
            offsetX += 1;
        }
    }
}

void RenderManager::RenderLine(int x1, int y1, int x2, int y2, Color color) {
    SDL_SetRenderDrawColor(&_renderer, color.red, color.green, color.blue, color.alpha);

    SDL_RenderDrawLine(&_renderer, x1, y1, x2, y2);
}

void RenderManager::RenderChip(const ChipDrawData& chipDrawData) {
    auto pos = chipDrawData.position;
    int width = 100;
    int heightFactor = 30;
    int height = chipDrawData.inputs > chipDrawData.outputs? chipDrawData.inputs * heightFactor : chipDrawData.outputs * heightFactor;

    RenderRect(pos.x, pos.y, width, height, {200, 100, 50, 255}, true);

    int circleSize = 20;
    int heightStepInputs = height / chipDrawData.inputs;
    int heightStepOutputs = height / chipDrawData.outputs;

    for(int i = 0; i < chipDrawData.inputs; i++){
        RenderCircle(pos.x - (width/2), (pos.y + (heightStepInputs * i) + (heightStepInputs/2)) - (height/2), circleSize, {30, 30, 30, 255});
    }

    for(int i = 0; i < chipDrawData.outputs; i++){
        RenderCircle(pos.x + (width/2), (pos.y + (heightStepOutputs * i) + (heightStepOutputs/2)) - (height/2), circleSize, {30, 30, 30, 255});
    }
}

