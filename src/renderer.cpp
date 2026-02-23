#include "renderer.h"
#include "globals.h"
#include <string>

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::RenderFrame(const CameraManager& cameraSystem, const Kart* player, const Track* track, const DecorationManager* deco, bool isPaused) {
    BeginDrawing();
        ClearBackground(SKYBLUE);
        
        BeginMode3D(cameraSystem.GetInternalCamera());
            
            DrawPlane({0, -0.1f, 0}, {2000, 2000}, DARKGREEN);
            
            if (track) track->Draw();
            if (deco) deco->Draw();
            if (player) player->Draw();
            
        EndMode3D();

        ui.DrawGameUI(player);
        if (isPaused) ui.DrawPauseScreen();
        
    EndDrawing();
}

void Renderer::DrawWorld() {}