#include "engine.h"

Engine::Engine() : isPaused(false), shouldClose(false) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super Scratch Kart R");
    SetExitKey(KEY_NULL);
    SetTargetFPS(TARGET_FPS);
    player = new Kart((Vector3){ 0.0f, 0.0f, 0.0f });
    audioSystem.PlayMusic();
}

Engine::~Engine() {
	delete player;
    CloseWindow();
}

void Engine::Update() {
	// Toggle Pause with Escape
    if (IsKeyPressed(KEY_ESCAPE)) {
        isPaused = !isPaused;
        
        if (isPaused) {
            SetMasterVolume(0.3f); // Lower volume to 30%
        } else {
            SetMasterVolume(1.0f); // Restore full volume
        }
    }

    if (!isPaused) {
        player->Update();
        cameraSystem.Update(player->GetPosition(), player->GetRotation());
        audioSystem.Update(player->GetSpeed(), 45.0f);
    }
}

void Engine::Draw() {
    renderer.RenderFrame(cameraSystem, player, isPaused);
}


void Engine::Run() {
    while (!WindowShouldClose() && !shouldClose) {
        Update();
        Draw();
    }
}