#include "engine.h"

Engine::Engine() : shouldClose(false) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super Scratch Kart R");
    SetTargetFPS(TARGET_FPS);
    player = new Kart((Vector3){ 0.0f, 0.0f, 0.0f });
    audioSystem.PlayMusic();
}

Engine::~Engine() {
	delete player;
    CloseWindow();
}

void Engine::Update() {
	player->Update();
    cameraSystem.Update(player->GetPosition(), player->GetRotation());
    audioSystem.Update(player->GetSpeed(), 45.0f);
}

void Engine::Draw() {
    renderer.RenderFrame(cameraSystem, player);
}


void Engine::Run() {
    while (!WindowShouldClose() && !shouldClose) {
        Update();
        Draw();
    }
}