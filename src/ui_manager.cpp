#include "ui_manager.h"
#include "font_manager.h"
#include "globals.h"
#include <vector>
#include <string>

struct ControlRow {
    std::string key;
    std::string action;
    Color actionColor;
};

UIManager::UIManager() {}

void UIManager::DrawCustomText(const char* text, Vector2 pos, float fontSize, Color color) {
    DrawTextEx(FontManager::GetMainFont(), text, pos, fontSize, 1.0f, color);
}

static void RenderControlRow(int x, int y, int keyColWidth, const ControlRow& row, float fontSize) {
    Font font = FontManager::GetMainFont();
    DrawTextEx(font, row.key.c_str(), {(float)x, (float)y}, fontSize, 1.0f, RAYWHITE);
    DrawTextEx(font, row.action.c_str(), {(float)(x + keyColWidth + 40), (float)y}, fontSize, 1.0f, row.actionColor);
}

void UIManager::DrawGameUI(const Kart* player) {
    DrawRectangle(0, 0, GetScreenWidth(), 45, Fade(BLACK, 0.5f));
    DrawCustomText("SUPER SCRATCH KART R", {20, 10}, 32, RAYWHITE);
    
    std::string fpsText = std::to_string(GetFPS()) + " FPS";
    Vector2 fpsSize = MeasureTextEx(FontManager::GetMainFont(), fpsText.c_str(), 24, 1);
    DrawCustomText(fpsText.c_str(), {(float)(GetScreenWidth() - fpsSize.x - 20), 12}, 24, LIME);
    
    const char* buildText = "Development Build 7";
    Vector2 buildSize = MeasureTextEx(FontManager::GetMainFont(), buildText, 18, 1);
    DrawCustomText(buildText, {(float)(GetScreenWidth() - buildSize.x - 20), (float)(GetScreenHeight() - 35)}, 18, Fade(RAYWHITE, 0.4f));
        
    if (player) {
        int speed = (int)player->GetSpeed();
        std::string speedStr = "SPEED: " + std::to_string(speed) + " KM/H";
        DrawCustomText(speedStr.c_str(), {20, (float)(GetScreenHeight() - 55)}, 40, YELLOW);
    }
}

void UIManager::DrawPauseScreen() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));
    
    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;

    const char* title = "PAUSED";
    Vector2 titleSize = MeasureTextEx(FontManager::GetMainFont(), title, 60, 1);
    DrawCustomText(title, {(float)(centerX - titleSize.x / 2), (float)(centerY - 220)}, 60, YELLOW);

    DrawControlsTable(centerX, centerY);

    const char* hint = "Press ESC to Resume";
    Vector2 hintSize = MeasureTextEx(FontManager::GetMainFont(), hint, 24, 1);
    DrawCustomText(hint, {(float)(centerX - hintSize.x / 2), (float)(centerY + 230)}, 24, DARKGRAY);
}

void UIManager::DrawControlsTable(int centerX, int centerY) {
    std::vector<ControlRow> controls = {
        {"W / UP", "ACCELERATE", LIME},
        {"S / DOWN", "BRAKE/REV", LIME},
        {"A-D / LEFT-RIGHT", "STEER", LIME},
        {"SHIFT", "DRIFT", LIME},
        {"Z", "HOP", LIME},
        {"ESC", "RESUME", GRAY}
    };

    float fontSize = 24.0f;
    float titleFontSize = 32.0f;
    int padding = 50;
    int margin = 35; 
    int lineSpacing = 38;

    Font font = FontManager::GetMainFont();
    int maxKeyWidth = 0;
    int maxActionWidth = 0;

    for (const auto& ctrl : controls) {
        int kw = MeasureTextEx(font, ctrl.key.c_str(), fontSize, 1).x;
        int aw = MeasureTextEx(font, ctrl.action.c_str(), fontSize, 1).x;
        if (kw > maxKeyWidth) maxKeyWidth = kw;
        if (aw > maxActionWidth) maxActionWidth = aw;
    }

    int boxW = maxKeyWidth + maxActionWidth + padding + (margin * 2);
    int boxH = (controls.size() * lineSpacing) + 110; 
    
    int boxX = centerX - boxW / 2;
    int boxY = centerY - boxH / 2;

    Rectangle boxRect = {(float)boxX, (float)boxY, (float)boxW, (float)boxH};
    DrawRectangleRounded(boxRect, 0.15f, 8, Fade(BLACK, 0.8f));
    DrawRectangleRoundedLines(boxRect, 0.15f, 8, YELLOW);

    const char* title = "CONTROLS";
    Vector2 tSize = MeasureTextEx(font, title, titleFontSize, 1);
    DrawCustomText(title, {(float)(centerX - tSize.x / 2), (float)(boxY + 25)}, titleFontSize, YELLOW);

    int startY = boxY + 85;
    int startX = boxX + margin;
    
    for (size_t i = 0; i < controls.size(); i++) {
        RenderControlRow(startX, startY + (i * lineSpacing), maxKeyWidth, controls[i], fontSize);
    }
}