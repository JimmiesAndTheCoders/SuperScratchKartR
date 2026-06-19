#include "engine.h"
#include "../editor/level_editor.h"
#include <cstring>

int main(int argc, char** argv) {
    bool launchEditor = false;
    bool dumpFrames = false;
    const char* dumpDir = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--editor") == 0 || strcmp(argv[i], "-e") == 0) {
            launchEditor = true;
        }
        const char* prefix = "--dump-frames=";
        if (strncmp(argv[i], prefix, strlen(prefix)) == 0) {
            dumpFrames = true;
            dumpDir = argv[i] + strlen(prefix);
        }
    }

    if (launchEditor) {
        LevelEditor editor(dumpFrames, dumpDir);
        editor.Run();
        return 0;
    }

    Engine game;
    game.Run();
    return 0;
}