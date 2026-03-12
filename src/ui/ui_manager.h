#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "raylib.h"
#include "kart.h"
#include <string>
#include <vector>

struct ControlRow {
    std::string key;
    std::string action;
    Color actionColor;
};

class UIManager {
public:
    UIManager();
    void DrawGameUI(const Kart* player);
    void DrawPauseScreen();
    
private:
    void DrawCustomText(const char* text, Vector2 pos, float fontSize, Color color);
    void DrawControlsTable(int centerX, int centerY);
};

#endif