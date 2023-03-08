//
// Created by Conner on 2/27/2023.
//

#ifndef LOGIC_SIM_RENDERMANAGER_H
#define LOGIC_SIM_RENDERMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <map>
#include "../Data/Color.h"
#include "../Data/ChipDrawData.h"
#include "../SimLogic/Button.h"

#define NodeOffStateColor Color(40,40,40,255)
#define NodeOnStateColor Color(230, 70, 70, 255)

class RenderManager {
public:
    explicit RenderManager(SDL_Renderer& renderer, int windowWidth, int windowHeight);

    void RenderRect(int x, int y, int w, int h, Color color = {}, bool fill = false);
    void RenderCircle(int x, int y, int radius, Color color = {});
    void RenderLine(int x1, int y1, int x2, int y2, Color color = {});
    void RenderText(const std::string& text, const std::string& font, int fontSize, Vector2 position, Color color = {});

    void RenderChip(const ChipDrawData& chipDrawData);
    void RenderIONodes(const std::vector<std::unique_ptr<IONode>>& nodes);
    void RenderChipInternal(const ChipDrawData& chipDrawData);
    void RenderWires(const std::vector<std::unique_ptr<Wire>>& wires);

    void RenderButton(const Button& button);

    void UpdateWindowSize(int windowWidth, int windowHeight);
    [[nodiscard]] Vector2 WindowSize() const;

    void AddFontResource(const std::string& fontName, const std::string& fontPath);
    TTF_Font& LoadFont(const std::string& fontName, int fontSize);
private:
    static Color GetNodeDrawColor(bool state);
private:
    SDL_Renderer& _renderer;
    int _windowWidth, _windowHeight;

    std::map<std::string, std::string> _fontResources;
    std::map<std::pair<std::string, int>, std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>> _loadedFonts;
};


#endif //LOGIC_SIM_RENDERMANAGER_H
