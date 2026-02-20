#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "camera_manager.h"
#include "kart.h"
#include "ui_manager.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    // The main function to draw everything
    void RenderFrame(const CameraManager& cameraSystem, const Kart* player, bool isPaused);

private:
    void DrawWorld();
    UIManager ui;
};

#endif