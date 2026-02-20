#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"
#include "camera_manager.h"
#include "globals.h"

class Engine {
    public:
        Engine();
        ~Engine();
        
        void Run();
    
    private:
        void Update();
        void Draw();
        
        CameraManager cameraSystem;
        Vector3 cubePosition;
        bool shouldClose;
};

#endif