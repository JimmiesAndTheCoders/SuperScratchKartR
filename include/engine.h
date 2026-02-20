#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"

class Engine {
    public:
        Engine();
        ~Engine();
        
        void Run();
        void Update();
        void Draw();
    
    private:
        const int screenWidth = 1280;
        const int screenHeight = 720;
        bool shouldClose;
};

#endif