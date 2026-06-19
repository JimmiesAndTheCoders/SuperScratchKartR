#include "level_editor.h"

#include "raymath.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"
#pragma GCC diagnostic pop

LevelEditor::LevelEditor(bool dumpFramesFlag, const char* dumpDir)
    : shouldClose(false)
    , gridEnabled(true)
    , fileDirty(false)
    , fileDialogOpen(false)
    , fileDialogSaveMode(false)
    , fileDialogSelectedIndex(-1)
    , currentTrack(nullptr)
    , menu()
    , scene() {
    dumpFrames = dumpFramesFlag;
    if (dumpDir && dumpDir[0] != '\0') {
        strncpy(dumpFramesDir, dumpDir, sizeof(dumpFramesDir) - 1);
        dumpFramesDir[sizeof(dumpFramesDir) - 1] = '\0';
    }
    currentFile[0] = '\0';
    currentTrackPath[0] = '\0';
    fileDialogDirectory[0] = '\0';
    fileDialogFilename[0] = '\0';
    InitWindow(800, 600, "Super Scratch Kart R - Level Editor");
    SetTargetFPS(60);
    powerManager.LoadModel();
    scene.SetEditor(this);
}

LevelEditor::~LevelEditor() {
    if (currentTrack) {
        delete currentTrack;
        currentTrack = nullptr;
    }
    CloseWindow();
}

void LevelEditor::HandleAction(MenuAction action) {
    switch (action) {
        case MenuAction::New:
            CreateNewLevel();
            break;
        case MenuAction::Open:
            OpenFileBrowser(false);
            break;
        case MenuAction::Save:
            if (currentFile[0] != '\0') {
                if (SaveLevelFile(currentFile)) {
                    std::cout << "[Editor] Saved " << currentFile << std::endl;
                    fileDirty = false;
                }
            } else {
                OpenFileBrowser(true);
            }
            break;
        case MenuAction::SaveAs:
            OpenFileBrowser(true);
            break;
        case MenuAction::Undo:
            std::cout << "[Editor] Undo" << std::endl;
            break;
        case MenuAction::ToggleGrid:
            gridEnabled = !gridEnabled;
            std::cout << "[Editor] Grid " << (gridEnabled ? "enabled" : "disabled") << std::endl;
            break;
        case MenuAction::GeneratePowerUps:
            if (currentTrack) {
                powerManager.GenerateCapsules(currentTrack, 50, 1500.0f);
                fileDirty = true;
                std::cout << "[Editor] Random power-ups generated" << std::endl;
            } else {
                std::cout << "[Editor] No track imported yet." << std::endl;
            }
            break;
        case MenuAction::ShowHelp:
            std::cout << "[Editor] Show help" << std::endl;
            break;
        default:
            break;
    }
}

void LevelEditor::Update() {
    MenuAction action = MenuAction::None;
    menu.Update(shouldClose, action);
    if (action != MenuAction::None) {
        HandleAction(action);
    }

    bool isMouseOverScene = GetMouseY() > menu.GetMenuHeight();
    scene.Update(gridEnabled, isMouseOverScene);
}

void LevelEditor::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    scene.Draw(menu.GetMenuHeight());
    menu.Draw();

    if (fileDialogOpen) {
        DrawFileBrowser();
    }

    DrawRectangle(0, GetScreenHeight() - 24, GetScreenWidth(), 24, LIGHTGRAY);
    char statusText[512];
    std::snprintf(statusText, sizeof(statusText), "File: %s %s | Ctrl+N New | Ctrl+O Open | Ctrl+S Save | Ctrl+Z Undo | F1 Help | ESC Exit",
        currentFile[0] ? currentFile : "[unnamed]",
        fileDirty ? "(modified)" : "");
    DrawText(statusText, 10, GetScreenHeight() - 20, 12, DARKGRAY);

    EndDrawing();

    // Optional frame dump (headless frame capture for stitching GIFs)
    if (dumpFrames) {
        if (dumpFramesDir[0] == '\0') {
            strncpy(dumpFramesDir, "tools/frame_dumps", sizeof(dumpFramesDir) - 1);
            dumpFramesDir[sizeof(dumpFramesDir) - 1] = '\0';
        }
        if (!DirectoryExists(dumpFramesDir)) MakeDirectory(dumpFramesDir);
        char buf[512];
        std::snprintf(buf, sizeof(buf), "%s/frame_%05d.png", dumpFramesDir, dumpFrameCounter++);
        Image img = GetScreenData();
        ExportImage(img, buf);
        UnloadImage(img);
    }
}

void LevelEditor::Run() {
    while (!WindowShouldClose() && !shouldClose) {
        Update();
        Draw();
    }
}
