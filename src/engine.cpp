#include "engine.h"
#include "renderer.h"

Engine::Engine() : shouldClose(false) {
    InitWindow(screenWidth, screenHeight, "Super Scratch Kart R");
    SetTargetFPS(60);
}

Engine::~Engine() {
    CloseWindow();
}

void Engine::Update() {
    
}

void Engine::Draw() {
    ClearBackground(RAYWHITE);
}


void Engine::Run() {
    Renderer renderer;
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    while (!WindowShouldClose()) {
        Update();
        renderer.UpdateCamera(cubePosition);
        
        BeginDrawing();
            Draw();
            
            renderer.DrawScene(cubePosition);
            renderer.DrawUI();
        EndDrawing();
    }
}