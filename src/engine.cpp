#include "engine.h"

Engine::Engine() : shouldClose(false) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super Scratch Kart R");
    SetTargetFPS(TARGET_FPS);
    player = new Kart((Vector3){ 0.0f, 0.0f, 0.0f });
}

Engine::~Engine() {
	delete player;
    CloseWindow();
}

void Engine::Update() {
	player->Update();
    cameraSystem.Update(player->GetPosition(), player->GetRotation());
}

void Engine::Draw() {
    BeginDrawing();
        ClearBackground(SKYBLUE); // Changed to SKYBLUE for better contrast
        BeginMode3D(cameraSystem.GetInternalCamera());
            DrawGrid(100, 10.0f);
            DrawPlane({0, -0.5f, 0}, {1000, 1000}, DARKGREEN);
            player->Draw();
        EndMode3D();
        
        // UI Header
        DrawRectangle(0, 0, SCREEN_WIDTH, 45, Fade(BLACK, 0.5f));
        DrawText("SUPER SCRATCH KART R", 20, 12, 20, RAYWHITE);
        DrawText(TextFormat("Development Build 1"), 1060, SCREEN_HEIGHT - 40, 20, Fade(RAYWHITE, 0.5f));
        
        // Speedometer
        int speed = (int)player->GetSpeed();
        DrawText(TextFormat("SPEED: %i", speed), 20, SCREEN_HEIGHT - 40, 30, RAYWHITE);
        
        DrawFPS(SCREEN_WIDTH - 100, 10);
    EndDrawing();
}


void Engine::Run() {
    while (!WindowShouldClose() && !shouldClose) {
        Update();
        Draw();
    }
}