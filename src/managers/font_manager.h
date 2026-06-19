#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "raylib.h"

class FontManager {
public:
    static void LoadFonts();
    static void UnloadFonts();
    static Font GetMainFont();

private:
    static Font mainFont;
    static bool isLoaded;
};

#endif