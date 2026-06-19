#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "raylib.h"
#include "level_editor_menu.h"
#include "level_editor_scene.h"
#include "../graphics/track.h"
#include "../managers/powerup_manager.h"

#include <string>
#include <vector>

class LevelEditor {
public:
    LevelEditor();
    ~LevelEditor();

    void Run();
    Track* GetTrack() { return currentTrack; }
    PowerUpManager& GetPowerManager() { return powerManager; }

private:
    void Update();
    void Draw();
    void HandleAction(MenuAction action);
    bool LoadLevelFile(const char* filename);
    bool SaveLevelFile(const char* filename);
    bool ImportTrack(const char* trackPath);
    void CreateNewLevel();
    void OpenFileBrowser(bool saveMode);
    void CloseFileBrowser();
    void PopulateFileBrowser();
    void DrawFileBrowser();
    bool TryLaunchSelectedBrowserEntry();

    bool shouldClose;
    bool gridEnabled;
    char currentFile[260];
    char currentTrackPath[260];
    bool fileDirty;
    bool fileDialogOpen;
    bool fileDialogSaveMode;
    char fileDialogDirectory[260];
    char fileDialogFilename[260];
    int fileDialogSelectedIndex;
    std::string fileDialogListText;
    std::vector<std::string> fileDialogEntries;
    Track* currentTrack;
    PowerUpManager powerManager;
    LevelEditorMenu menu;
    LevelEditorScene scene;
};

#endif
