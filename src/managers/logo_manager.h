#ifndef LOGO_MANAGER_H
#define LOGO_MANAGER_H

#include "raylib.h"

class LogoManager {
public:
    static void LoadLogo(int targetHeight);
    static void UnloadLogo();
    static Texture2D GetLogo();
    static float GetWidthForHeight(float height);

private:
    static Texture2D logoTexture;
    static bool isLoaded;
};

#endif