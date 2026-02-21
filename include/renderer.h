#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "camera_manager.h"
#include "kart.h"
#include "ui_manager.h"
#include "track.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void RenderFrame(const CameraManager& cameraSystem, const Kart* player, const Track* track, bool isPaused);

private:
    void DrawWorld();
    UIManager ui;
};

#endif