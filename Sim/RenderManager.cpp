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
    int width = chipDrawData.extends.x;
    int height = chipDrawData.extends.y;

    RenderRect(pos.x, pos.y, width, height, {50, 100, 200, 255}, true);

    for(auto& inputNode : chipDrawData.inputs){
        Color nodeColor = GetNodeDrawColor(inputNode.State());
        RenderCircle(inputNode.Position().x, inputNode.Position().y, inputNode.AABBExtends().x, nodeColor);
    }

    for(auto& outputNode : chipDrawData.outputs){
        Color nodeColor = GetNodeDrawColor(outputNode.State());
        RenderCircle(outputNode.Position().x, outputNode.Position().y, outputNode.AABBExtends().x, nodeColor);
    }

//    int circleSize = chipDrawData.inputs.at(0).AABBExtends().x;
//    int inputAmount = static_cast<int>(chipDrawData.inputs.size());
//    int outputAmount = static_cast<int>(chipDrawData.outputs.size());
//    int heightStepInputs = height / inputAmount;
//    int heightStepOutputs = height / outputAmount;
//
//    for(int i = 0; i < inputAmount; i++){
//        Color nodeColor = GetNodeDrawColor(chipDrawData.inputs[i].State());
//        RenderCircle(pos.x - (width/2), (pos.y + (heightStepInputs * i) + (heightStepInputs/2)) - (height/2), circleSize, nodeColor);
//    }
//
//    for(int i = 0; i < outputAmount; i++){
//        Color nodeColor = GetNodeDrawColor(chipDrawData.outputs[i].State());
//        RenderCircle(pos.x + (width/2), (pos.y + (heightStepOutputs * i) + (heightStepOutputs/2)) - (height/2), circleSize, nodeColor);
//    }
}

Color RenderManager::GetNodeDrawColor(bool state) {
    if(state)
        return NodeOnStateColor;
    else
        return NodeOffStateColor;
}

