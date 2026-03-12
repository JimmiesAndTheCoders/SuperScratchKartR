#include "logo_manager.h"
#include <iostream>

Texture2D LogoManager::logoTexture = { 0 };
bool LogoManager::isLoaded = false;

void LogoManager::LoadLogo(int targetHeight) {
    if (!isLoaded) {
        Image logoImg = LoadImage("assets/images/ui/sskrlogo.png");

        if (logoImg.data != nullptr) {
            logoTexture = LoadTextureFromImage(logoImg);
            GenTextureMipmaps(&logoTexture);
            SetTextureFilter(logoTexture, TEXTURE_FILTER_TRILINEAR);
            
            UnloadImage(logoImg);
            isLoaded = true;
            std::cout << "LOG: High-Quality Logo loaded with Trilinear filtering." << std::endl;
        } else {
            std::cerr << "ERROR: Could not find assets/images/ui/sskrlogo.png" << std::endl;
        }
    }
}

void LogoManager::UnloadLogo() {
    if (isLoaded) {
        UnloadTexture(logoTexture);
        logoTexture = (Texture2D){ 0 };
        isLoaded = false;
    }
}

Texture2D LogoManager::GetLogo() {
    return logoTexture;
}

float LogoManager::GetWidthForHeight(float height) {
    if (!isLoaded || logoTexture.height == 0) return 0;
    float aspect = (float)logoTexture.width / (float)logoTexture.height;
    return height * aspect;
}