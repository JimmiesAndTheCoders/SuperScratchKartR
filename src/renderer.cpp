#include "renderer.h"
#include "globals.h"
#include <string>

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::RenderFrame(const CameraManager& cameraSystem, const Kart* player, const Track* track, bool isPaused) {
    BeginDrawing();
        ClearBackground(SKYBLUE);
        
        BeginMode3D(cameraSystem.GetInternalCamera());
            
            DrawPlane({0, -0.1f, 0}, {1000, 1000}, DARKGREEN);
            
            if (track) track->Draw();
            if (player) player->Draw();
            
        EndMode3D();

        ui.DrawGameUI(player);
        if (isPaused) ui.DrawPauseScreen();
        
    EndDrawing();
}

void Renderer::DrawWorld() {}