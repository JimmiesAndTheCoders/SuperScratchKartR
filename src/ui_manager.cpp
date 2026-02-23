#include "ui_manager.h"
#include "font_manager.h"
#include "logo_manager.h"
#include "globals.h"

UIManager::UIManager() {}

void UIManager::DrawCustomText(const char* text, Vector2 pos, float fontSize, Color color) {
    DrawTextEx(FontManager::GetMainFont(), text, pos, fontSize, 1.0f, color);
}

void UIManager::DrawGameUI(const Kart* player) {
    Texture2D logo = LogoManager::GetLogo();
    if (logo.id != 0) {
        float targetHeight = 75.5f;
        float scale = targetHeight / (float)logo.height;
        DrawTextureEx(logo, {20, 20}, 0.0f, scale, WHITE);
    }
    
    std::string fpsText = std::to_string(GetFPS()) + " FPS";
    Vector2 fpsSize = MeasureTextEx(FontManager::GetMainFont(), fpsText.c_str(), 24, 1);
    DrawCustomText(fpsText.c_str(), {(float)(GetScreenWidth() - fpsSize.x - 20), 18}, 24, LIME);
    
    const char* buildText = "Development Build 5";
    Vector2 buildSize = MeasureTextEx(FontManager::GetMainFont(), buildText, 18, 1);
    DrawCustomText(buildText, {(float)(GetScreenWidth() - buildSize.x - 20), (float)(GetScreenHeight() - 35)}, 18, Fade(RAYWHITE, 0.4f));
        
    if (player) {
        int speed = (int)player->GetSpeed();
        std::string speedStr = "SPEED: " + std::to_string(speed) + " KM/H";
        DrawCustomText(speedStr.c_str(), {20, (float)(GetScreenHeight() - 55)}, 40, YELLOW);
    }
}

void UIManager::DrawPauseScreen() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.75f));
    
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();
    int centerX = screenW / 2;

    float logoTargetHeight = 140.0f;
    float tableTargetHeight = 350.0f;
    float spacing = 30.0f;
    float hintFontSize = 24.0f;
    float totalContentHeight = logoTargetHeight + spacing + tableTargetHeight + spacing + hintFontSize;
    float currentY = (screenH - totalContentHeight) / 2.0f;

    Texture2D logo = LogoManager::GetLogo();
    if (logo.id != 0) {
        float scale = logoTargetHeight / (float)logo.height;
        Vector2 logoPos = {
            (float)centerX - (logo.width * scale) / 2.0f,
            currentY
        };
        DrawTextureEx(logo, logoPos, 0.0f, scale, WHITE);
    }
    
    currentY += logoTargetHeight + spacing;
    DrawControlsTable(centerX, (int)currentY);
    
    currentY += tableTargetHeight + spacing;
    const char* hint = "Press ESC to Resume";
    Vector2 hintSize = MeasureTextEx(FontManager::GetMainFont(), hint, hintFontSize, 1);
    DrawCustomText(hint, {(float)(centerX - hintSize.x / 2), currentY}, hintFontSize, LIGHTGRAY);
}

void UIManager::DrawControlsTable(int centerX, int topY) {
    std::vector<ControlRow> controls = {
        {"W / UP", "ACCELERATE", LIME},
        {"S / DOWN", "BRAKE/REV", LIME},
        {"A-D / LEFT-RIGHT", "STEER", LIME},
        {"SHIFT", "DRIFT", LIME},
        {"Z", "HOP", LIME},
        {"ESC", "RESUME", GRAY}
    };

    float fontSize = 24.0f;
    float titleFontSize = 30.0f;
    int padding = 40;
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
    int boxH = (controls.size() * lineSpacing) + 100; 
    
    int boxX = centerX - boxW / 2;
    int boxY = topY;

    Rectangle boxRect = {(float)boxX, (float)boxY, (float)boxW, (float)boxH};
    DrawRectangleRounded(boxRect, 0.1f, 8, Fade(BLACK, 0.85f));
    DrawRectangleRoundedLines(boxRect, 0.1f, 8, YELLOW);

    const char* title = "CONTROLS";
    Vector2 tSize = MeasureTextEx(font, title, titleFontSize, 1);
    DrawCustomText(title, {(float)(centerX - tSize.x / 2), (float)(boxY + 20)}, titleFontSize, YELLOW);

    int startY = boxY + 75;
    int startX = boxX + margin;
    
    for (size_t i = 0; i < controls.size(); i++) {
        DrawTextEx(font, controls[i].key.c_str(), {(float)startX, (float)(startY + (i * lineSpacing))}, fontSize, 1.0f, RAYWHITE);
        DrawTextEx(font, controls[i].action.c_str(), {(float)(startX + maxKeyWidth + 40), (float)(startY + (i * lineSpacing))}, fontSize, 1.0f, controls[i].actionColor);
    }
}