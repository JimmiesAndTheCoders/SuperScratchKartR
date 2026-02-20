#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "raylib.h"
#include "kart.h"

class UIManager {
public:
    UIManager();
    void DrawGameUI(const Kart* player);
    void DrawPauseScreen();

private:
    void DrawControlsTable(int centerX, int centerY);
};

#endif