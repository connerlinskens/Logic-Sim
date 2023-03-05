//
// Created by Conner on 2/27/2023.
//

#include "RenderManager.h"
#include <cmath>

#define WindowWidthMargin 50
#define WindowHeightMargin 125

RenderManager::RenderManager(SDL_Renderer& renderer, int windowWidth, int windowHeight) : _renderer{renderer}, _windowWidth{windowWidth}, _windowHeight{windowHeight} {
}

void RenderManager::UpdateWindowSize(int windowWidth, int windowHeight) {
    _windowWidth = windowWidth;
    _windowHeight = windowHeight;
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

    RenderRect(pos.x, pos.y, width, height, chipDrawData.drawColor, true);
}

void RenderManager::RenderIONodesWithWires(const std::vector<IONode> &nodes) {
    for(auto& node : nodes){
        // Render IONode
        Color nodeColor = GetNodeDrawColor(node.State());
        RenderCircle(node.Position().x, node.Position().y, node.AABBExtends().x, nodeColor);

        // Render wires
        auto& wires = node.Wires();
        for(auto& wire : wires){
            auto attachedNodePosition = wire.InputNode().Position();
            RenderLine(node.Position().x, node.Position().y, attachedNodePosition.x, attachedNodePosition.y, nodeColor);
        }
    }
}

Color RenderManager::GetNodeDrawColor(bool state) {
    if(state)
        return NodeOnStateColor;
    else
        return NodeOffStateColor;
}

void RenderManager::RenderChipInternal(const ChipDrawData& chipDrawData) {
    // Draw chip borders
    Vector2 chipPos = {_windowWidth/2, _windowHeight/2};
    Vector2 chipExtends = {_windowWidth - WindowWidthMargin, _windowHeight - WindowHeightMargin};
    RenderRect(chipPos.x, chipPos.y, chipExtends.x, chipExtends.y, {150, 150, 150, 255}, false);
}

Vector2 RenderManager::WindowSize() const {
    return {_windowWidth, _windowHeight};
}
