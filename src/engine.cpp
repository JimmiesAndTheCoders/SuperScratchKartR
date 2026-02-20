#include "engine.h"

Engine::Engine() : shouldClose(false) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super Scratch Kart R");
    SetTargetFPS(TARGET_FPS);
    
    // Init game data
    cubePosition = { 0.0f, 0.0f, 0.0f };
}

Engine::~Engine() {
    CloseWindow();
}

void Engine::Update() {
    if (IsKeyDown(KEY_RIGHT)) cubePosition.x += 0.1f;
    if (IsKeyDown(KEY_LEFT))  cubePosition.x -= 0.1f;

    cameraSystem.Update(cubePosition);
}

void Engine::Draw() {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(cameraSystem.GetInternalCamera());
            DrawGrid(20, 1.0f);
            DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        EndMode3D();
        DrawText("Super Scratch Kart R - Dev Build 0.5", 10, 10, 20, DARKGRAY);
        DrawFPS(10, 40);
    EndDrawing();
}


void Engine::Run() {
    while (!WindowShouldClose() && !shouldClose) {
        Update();
        Draw();
    }
}