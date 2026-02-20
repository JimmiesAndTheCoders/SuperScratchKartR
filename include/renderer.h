#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"

class Renderer {
    public:
        Renderer();
        
        void UpdateCamera(Vector3 target);
        void DrawScene(Vector3 cubePos);
        void DrawUI();
    
    private:
        Camera3D camera;
};

#endif