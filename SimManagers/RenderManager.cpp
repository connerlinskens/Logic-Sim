//
// Created by Conner on 2/27/2023.
//

#include "RenderManager.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

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
    RenderText(chipDrawData.name, "ShareTechMono", 20, chipDrawData.position);
}

void RenderManager::RenderIONodes(const std::vector<std::unique_ptr<IONode>> &nodes, bool internal) {
    if(!internal){
        for(auto& node : nodes){
            RenderCircle(node->Position().x, node->Position().y, node->AABBExtends().x, node->getColor());
        }
    }
    else{
        for(auto& node : nodes){
            RenderCircle(node->Position().x, node->Position().y, node->AABBExtends().x + 8, {120, 120, 120, 255});
            RenderCircle(node->Position().x, node->Position().y, node->AABBExtends().x + 5, node->getColor());
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

void RenderManager::RenderButton(const Button& button) {
    RenderRect(button.Position().x, button.Position().y, button.AABBExtends().x, button.AABBExtends().y, {200, 200, 200, 255}, false);
    RenderText(button.Text(), "ShareTechMono", 20, button.Position());
}

void RenderManager::RenderWires(const std::vector<std::unique_ptr<Wire>>& wires) {
    for(auto& wire : wires){
        Color wireColor = GetNodeDrawColor(wire->NodeA().State() && wire->NodeB().State());
        Vector2 posA = wire->NodeA().Position();
        Vector2 posB = wire->NodeB().Position();
        RenderLine(posA.x, posA.y, posB.x, posB.y, wireColor);
    }
}

void RenderManager::AddFontResource(const std::string& name, const std::string& path) {
    _fontResources.insert({name, path});
}

TTF_Font& RenderManager::LoadFont(const std::string& fontName, int fontSize) {
    auto loadedFont = _loadedFonts.find(std::make_pair(fontName, fontSize));
    if(loadedFont != _loadedFonts.end()){
        // Return cached font
        return *loadedFont->second;
    }
    else{
        // Load new font
        auto fontPath = _fontResources.find(fontName);
        if(fontPath != _fontResources.end()){
            auto newFont = TTF_OpenFont(fontPath->second.c_str(), fontSize);
            if(newFont){
                std::unique_ptr<TTF_Font, void(*)(TTF_Font*)> fontWrapper = {newFont, TTF_CloseFont};
                std::pair<std::string, int> fontKey = std::make_pair(fontName, fontSize);
                auto font = _loadedFonts.insert({fontKey, std::move(fontWrapper)});
                return *font.first->second;
            }
        }
    }
    // If no loaded font or resource was found, throw exception
    throw std::runtime_error("No resource found with given path. In LoadFont()");
}

void RenderManager::RenderText(const std::string& text, const std::string& font, int fontSize, Vector2 position, Color color) {
    if(text.empty()) {return;}

    auto& fontRef = LoadFont(font, fontSize);

    auto textSurface = TTF_RenderText_Solid(&fontRef, text.c_str(), {color.red, color.green, color.blue, color.alpha});
    if(!textSurface){
        std::cerr << "Unable to load text to surface, Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Rect dstRect{
        position.x - (textSurface->w/2),
        position.y - (textSurface->h/2),
        textSurface->w,
        textSurface->h };

    auto textTexture = SDL_CreateTextureFromSurface(&_renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if(!textTexture){
        std::cerr << "Unable to create texture from surface, Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_RenderCopy(&_renderer, textTexture, nullptr, &dstRect);
    SDL_DestroyTexture(textTexture);
}
