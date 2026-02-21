#include "ui_manager.h"
#include "globals.h"

UIManager::UIManager() {}

void UIManager::DrawGameUI(const Kart* player) {
    // UI Header
    DrawRectangle(0, 0, SCREEN_WIDTH, 45, Fade(BLACK, 0.5f));
    DrawText("SUPER SCRATCH KART R", 20, 12, 20, RAYWHITE);
    DrawText(TextFormat("Development Build 3"), 1060, SCREEN_HEIGHT - 40, 20, Fade(RAYWHITE, 0.5f));
        
    // Speedometer
    if (player) {
        int speed = (int)player->GetSpeed();
        DrawText(TextFormat("SPEED: %i KM/H", speed), 20, SCREEN_HEIGHT - 40, 30, YELLOW);
    }
	  
    DrawFPS(SCREEN_WIDTH - 100, 10);
}

void UIManager::DrawPauseScreen() {
    // Darken Background
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
    
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    // Pause Title
    const char* title = "PAUSED";
    DrawText(title, centerX - MeasureText(title, 40)/2, centerY - 150, 40, YELLOW);

    DrawControlsTable(centerX, centerY);

    const char* hint = "Press ESC to Resume";
    DrawText(hint, centerX - MeasureText(hint, 20)/2, centerY + 180, 20, DARKGRAY);
}

void UIManager::DrawControlsTable(int centerX, int centerY) {
    int boxW = 450;
    int boxH = 220;
    int boxX = centerX - boxW/2;
    int boxY = centerY - 60;

    // Table Background
    DrawRectangle(boxX, boxY, boxW, boxH, Fade(BLACK, 0.4f));
    DrawRectangleLines(boxX, boxY, boxW, boxH, YELLOW);

    int textX_Key = boxX + 30;
    int textX_Action = boxX + 265;
    int startY = boxY + 20;

    DrawText("CONTROLS", centerX - MeasureText("CONTROLS", 25)/2, startY, 25, YELLOW);
    
    DrawText("W / UP", textX_Key, startY + 50, 20, RAYWHITE);
    DrawText("ACCELERATE", textX_Action, startY + 50, 20, LIME);
    
    DrawText("S / DOWN", textX_Key, startY + 80, 20, RAYWHITE);
    DrawText("BRAKE/REV", textX_Action, startY + 80, 20, LIME);
    
    DrawText("A-D / LEFT-RIGHT", textX_Key, startY + 110, 20, RAYWHITE);
    DrawText("STEER", textX_Action, startY + 110, 20, LIME);

    DrawText("ESC", textX_Key, startY + 140, 20, RAYWHITE);
    DrawText("RESUME", textX_Action, startY + 140, 20, GRAY);
}