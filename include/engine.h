#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"
#include "camera_manager.h"
#include "globals.h"
#include "kart.h"

class Engine {
    public:
        Engine();
        ~Engine();
        
        void Run();
    
    private:
        void Update();
        void Draw();
        
        CameraManager cameraSystem;
        Kart* player;
        bool shouldClose;
};

#endif