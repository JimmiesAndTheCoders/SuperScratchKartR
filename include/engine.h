#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"
#include "audio_manager.h"
#include "camera_manager.h"
#include "renderer.h"
#include "globals.h"
#include "kart.h"
#include "track.h"

class Engine {
public:
    Engine();
    ~Engine();

    void Run();

private:
    void Update();
    void Draw();

    CameraManager cameraSystem;
    AudioManager audioSystem;
    Renderer renderer;
    Kart* player;
    Track* currentTrack;

    bool isPaused;
    bool shouldClose;
};

#endif