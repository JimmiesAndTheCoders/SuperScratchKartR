#include "renderer.h"
#include "globals.h"
#include <string>

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::RenderFrame(const CameraManager& cameraSystem, const Kart* player) {
    BeginDrawing();
        ClearBackground(SKYBLUE);
        // Draw 3D Scene
        BeginMode3D(cameraSystem.GetInternalCamera());
            DrawWorld();
            if (player) player->Draw();
        EndMode3D();
        // Draw 2D Overlay
        DrawUI(player);
    EndDrawing();
}

void Renderer::DrawWorld() {
    // Checkered ground feel
    DrawPlane({0, -0.5f, 0}, {1000, 1000}, DARKGREEN);
    DrawGrid(100, 10.0f);
}

void Renderer::DrawUI(const Kart* player) {
    // UI Header
    DrawRectangle(0, 0, SCREEN_WIDTH, 45, Fade(BLACK, 0.5f));
    DrawText("SUPER SCRATCH KART R", 20, 12, 20, RAYWHITE);
    DrawText(TextFormat("Development Build 2"), 1060, SCREEN_HEIGHT - 40, 20, Fade(RAYWHITE, 0.5f));
        
    // Speedometer
    int speed = (int)player->GetSpeed();
    DrawText(TextFormat("SPEED: %i", speed), 20, SCREEN_HEIGHT - 40, 30, RAYWHITE);
        
    DrawFPS(SCREEN_WIDTH - 100, 10);
}