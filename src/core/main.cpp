#include "engine.h"
#include "../editor/level_editor.h"
#include <cstring>

int main(int argc, char** argv) {
    bool launchEditor = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--editor") == 0 || strcmp(argv[i], "-e") == 0) {
            launchEditor = true;
            break;
        }
    }

    if (launchEditor) {
        LevelEditor editor;
        editor.Run();
        return 0;
    }

    Engine game;
    game.Run();
    return 0;
}