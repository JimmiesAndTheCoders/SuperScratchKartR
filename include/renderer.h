#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "camera_manager.h"
#include "kart.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    // The main function to draw everything
    void RenderFrame(const CameraManager& cameraSystem, const Kart* player);

private:
    void DrawWorld();
    void DrawUI(const Kart* player);
};

#endif