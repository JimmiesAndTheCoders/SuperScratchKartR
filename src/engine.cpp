#include "engine.h"

Engine::Engine() : isPaused(false), shouldClose(false) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super Scratch Kart R");
    SetExitKey(KEY_NULL);
    SetTargetFPS(TARGET_FPS);

    currentTrack = new Track("assets/models/tracks/test.glb");

    player = new Kart((Vector3){ 0.0f, 10.0f, 0.0f });
    
    audioSystem.PlayMusic();
}

Engine::~Engine() {
    delete player;
    delete currentTrack;
    CloseWindow();
}

void Engine::Update() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        isPaused = !isPaused;
        
        if (isPaused) {
            SetMasterVolume(0.3f);
        } else {
            SetMasterVolume(1.0f);
        }
    }

    if (!isPaused) {
        player->Update(currentTrack);
        cameraSystem.Update(player->GetPosition(), player->GetRotation());
        audioSystem.Update(player->GetSpeed(), 45.0f);
    }
}

void Engine::Draw() {
    renderer.RenderFrame(cameraSystem, player, currentTrack, isPaused);
}

void Engine::Run() {
    while (!WindowShouldClose() && !shouldClose) {
        Update();
        Draw();
    }
}