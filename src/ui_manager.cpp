#include "ui_manager.h"
#include "globals.h"
#include <vector>
#include <string>

struct ControlRow {
    std::string key;
    std::string action;
    Color actionColor;
};

static void RenderControlRow(int x, int y, int keyColWidth, const ControlRow& row, int fontSize) {
    DrawText(row.key.c_str(), x, y, fontSize, RAYWHITE);
    DrawText(row.action.c_str(), x + keyColWidth + 40, y, fontSize, row.actionColor);
}

UIManager::UIManager() {}

void UIManager::DrawGameUI(const Kart* player) {
    // UI Header
    DrawRectangle(0, 0, GetScreenWidth(), 45, Fade(BLACK, 0.5f));
    DrawText("SUPER SCRATCH KART R", 20, 12, 20, RAYWHITE);
    
    const char* buildText = "Development Build 5";
    int buildWidth = MeasureText(buildText, 20);
    DrawText(buildText, GetScreenWidth() - buildWidth - 20, GetScreenHeight() - 40, 20, Fade(RAYWHITE, 0.5f));
        
    // Speedometer
    if (player) {
        int speed = (int)player->GetSpeed();
        DrawText(TextFormat("SPEED: %i KM/H", speed), 20, GetScreenHeight() - 40, 30, YELLOW);
    }
      
    DrawFPS(GetScreenWidth() - 100, 10);
}

void UIManager::DrawPauseScreen() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
    
    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;

    const char* title = "PAUSED";
    DrawText(title, centerX - MeasureText(title, 40)/2, centerY - 180, 40, YELLOW);

    DrawControlsTable(centerX, centerY);

    const char* hint = "Press ESC to Resume";
    DrawText(hint, centerX - MeasureText(hint, 20)/2, centerY + 200, 20, DARKGRAY);
}

void UIManager::DrawControlsTable(int centerX, int centerY) {
    std::vector<ControlRow> controls = {
        {"W / UP", "ACCELERATE", LIME},
        {"S / DOWN", "BRAKE/REV", LIME},
        {"A-D / LEFT-RIGHT", "STEER", LIME},
        {"SHIFT", "DRIFT", LIME},
        {"ESC", "RESUME", GRAY}
    };

    int fontSize = 20;
    int titleFontSize = 25;
    int padding = 40;
    int margin = 30; 
    int lineSpacing = 30;

    int maxKeyWidth = 0;
    int maxActionWidth = 0;
    for (const auto& ctrl : controls) {
        int kw = MeasureText(ctrl.key.c_str(), fontSize);
        int aw = MeasureText(ctrl.action.c_str(), fontSize);
        if (kw > maxKeyWidth) maxKeyWidth = kw;
        if (aw > maxActionWidth) maxActionWidth = aw;
    }

    int tableWidth = maxKeyWidth + maxActionWidth + padding;
    int boxW = tableWidth + (margin * 2);
    int boxH = (controls.size() * lineSpacing) + 80; 
    
    int boxX = centerX - boxW / 2;
    int boxY = centerY - boxH / 2;

    DrawRectangle(boxX, boxY, boxW, boxH, Fade(BLACK, 0.4f));
    DrawRectangleLines(boxX, boxY, boxW, boxH, YELLOW);

    const char* title = "CONTROLS";
    DrawText(title, centerX - MeasureText(title, titleFontSize) / 2, boxY + 20, titleFontSize, YELLOW);

    int startY = boxY + 65;
    int startX = boxX + margin;
    
    for (size_t i = 0; i < controls.size(); i++) {
        RenderControlRow(startX, startY + (i * lineSpacing), maxKeyWidth, controls[i], fontSize);
    }
}