#include "font_manager.h"
#include <iostream>

Font FontManager::mainFont = { 0 };
bool FontManager::isLoaded = false;

void FontManager::LoadFonts() {
    if (!isLoaded) {
        mainFont = LoadFontEx("assets/fonts/Dimbo Regular.ttf", 96, 0, 0);
        SetTextureFilter(mainFont.texture, TEXTURE_FILTER_TRILINEAR);
        GenTextureMipmaps(&mainFont.texture);
        
        isLoaded = true;
    }
}

void FontManager::UnloadFonts() {
    if (isLoaded) {
        UnloadFont(mainFont);
        isLoaded = false;
    }
}

Font FontManager::GetMainFont() {
    // Fallback to default font if loading failed
    if (!isLoaded || mainFont.texture.id == 0) return GetFontDefault();
    return mainFont;
}