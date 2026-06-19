// Implements file browser UI and navigation for LevelEditor
#include "level_editor.h"

#include "../include/raygui.h"

#include <string>
#include <vector>
#include <iostream>
#include <cstring>

static std::string JoinPathLocal(const char* directory, const char* filename) {
    std::string path = directory;
    if (!path.empty() && path.back() != '/' && path.back() != '\\') path += '/';
    path += filename;
    return path;
}

static std::string GetParentDirectoryLocal(const std::string &path) {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(0, pos);
}

void LevelEditor::OpenFileBrowser(bool saveMode) {
    fileDialogOpen = true;
    fileDialogSaveMode = saveMode;
    fileDialogSelectedIndex = -1;
    if (currentFile[0] != '\0') {
        std::string fullPath(currentFile);
        fileDialogFilename[0] = '\0';
        fileDialogDirectory[0] = '\0';
        size_t sep = fullPath.find_last_of("/\\");
        if (sep != std::string::npos) {
            std::string dir = fullPath.substr(0, sep);
            strncpy(fileDialogDirectory, dir.c_str(), sizeof(fileDialogDirectory) - 1);
            fileDialogDirectory[sizeof(fileDialogDirectory) - 1] = '\0';
            std::string name = fullPath.substr(sep + 1);
            strncpy(fileDialogFilename, name.c_str(), sizeof(fileDialogFilename) - 1);
            fileDialogFilename[sizeof(fileDialogFilename) - 1] = '\0';
        } else {
            strncpy(fileDialogDirectory, "assets/levels", sizeof(fileDialogDirectory) - 1);
            fileDialogDirectory[sizeof(fileDialogDirectory) - 1] = '\0';
            strncpy(fileDialogFilename, fullPath.c_str(), sizeof(fileDialogFilename) - 1);
            fileDialogFilename[sizeof(fileDialogFilename) - 1] = '\0';
        }
    } else {
        strncpy(fileDialogDirectory, "assets/levels", sizeof(fileDialogDirectory) - 1);
        fileDialogDirectory[sizeof(fileDialogDirectory) - 1] = '\0';
        if (saveMode) {
            strncpy(fileDialogFilename, "untitled.sskrt", sizeof(fileDialogFilename) - 1);
            fileDialogFilename[sizeof(fileDialogFilename) - 1] = '\0';
        } else {
            fileDialogFilename[0] = '\0';
        }
    }
    PopulateFileBrowser();
}

void LevelEditor::CloseFileBrowser() {
    fileDialogOpen = false;
    fileDialogSelectedIndex = -1;
    fileDialogListText.clear();
}

void LevelEditor::PopulateFileBrowser() {
    fileDialogEntries.clear();
    if (!DirectoryExists(fileDialogDirectory)) {
        if (!MakeDirectory(fileDialogDirectory)) {
            return;
        }
    }

    FilePathList list = LoadDirectoryFiles(fileDialogDirectory);
    for (int i = 0; i < (int)list.count; ++i) {
        std::string name = list.paths[i];
        std::string fullPath = JoinPathLocal(fileDialogDirectory, name.c_str());
        if (DirectoryExists(fullPath.c_str())) {
            fileDialogEntries.push_back(std::string("[DIR] ") + name);
        } else if (IsFileExtension(name.c_str(), ".sskrt")) {
            fileDialogEntries.push_back(name);
        }
    }
    UnloadDirectoryFiles(list);
}

bool LevelEditor::TryLaunchSelectedBrowserEntry() {
    if (fileDialogSelectedIndex >= 0 && fileDialogSelectedIndex < (int)fileDialogEntries.size()) {
        const std::string &entry = fileDialogEntries[fileDialogSelectedIndex];
        if (entry.rfind("[DIR] ", 0) == 0) {
            std::string dirName = entry.substr(6);
            std::string newDir = JoinPathLocal(fileDialogDirectory, dirName.c_str());
            strncpy(fileDialogDirectory, newDir.c_str(), sizeof(fileDialogDirectory) - 1);
            fileDialogDirectory[sizeof(fileDialogDirectory) - 1] = '\0';
            PopulateFileBrowser();
            fileDialogSelectedIndex = -1;
            return false;
        }
        strncpy(fileDialogFilename, entry.c_str(), sizeof(fileDialogFilename) - 1);
        fileDialogFilename[sizeof(fileDialogFilename) - 1] = '\0';
    }

    if (fileDialogFilename[0] == '\0') {
        return false;
    }

    std::string selectedPath = JoinPathLocal(fileDialogDirectory, fileDialogFilename);
    if (fileDialogSaveMode) {
        if (SaveLevelFile(selectedPath.c_str())) {
            strncpy(currentFile, selectedPath.c_str(), sizeof(currentFile) - 1);
            currentFile[sizeof(currentFile) - 1] = '\0';
            fileDirty = false;
            std::cout << "[Editor] Saved " << currentFile << std::endl;
            CloseFileBrowser();
            return true;
        }
        return false;
    }

    if (LoadLevelFile(selectedPath.c_str())) {
        std::cout << "[Editor] Opened " << selectedPath << std::endl;
        CloseFileBrowser();
        return true;
    }
    return false;
}

void LevelEditor::DrawFileBrowser() {
    const float width = 620.0f;
    const float height = 420.0f;
    const float x = (GetScreenWidth() - width) * 0.5f;
    const float y = (GetScreenHeight() - height) * 0.5f;

    Rectangle window = { x, y, width, height };
    GuiWindowBox(window, fileDialogSaveMode ? "Save Level As" : "Open Level");

    DrawText("Directory:", x + 12, y + 28, 12, DARKGRAY);
    DrawText(fileDialogDirectory, x + 90, y + 28, 12, DARKGRAY);

    DrawText("File:", x + 12, y + 52, 12, DARKGRAY);
    Rectangle filenameBox = { x + 90, y + 48, width - 110, 24 };
    GuiTextBox(filenameBox, fileDialogFilename, sizeof(fileDialogFilename), true);

    float listY = y + 84;
    Rectangle listBox = { x + 12, listY, width - 24, height - 150 };
    DrawRectangleRec(listBox, LIGHTGRAY);
    DrawRectangleLinesEx(listBox, 1.0f, DARKGRAY);

    const float rowHeight = 28.0f;
    int visibleCount = (int)((listBox.height - 8) / rowHeight);
    int entryCount = (int)fileDialogEntries.size();
    int maxIndex = std::min(entryCount, visibleCount);
    for (int i = 0; i < maxIndex; ++i) {
        Rectangle entryRect = { listBox.x + 4, listBox.y + 4 + i * rowHeight, listBox.width - 8, rowHeight - 4 };
        if (GuiButton(entryRect, fileDialogEntries[i].c_str())) {
            fileDialogSelectedIndex = i;
        }
        if (fileDialogSelectedIndex == i) {
            DrawRectangleLines((int)entryRect.x, (int)entryRect.y, (int)entryRect.width, (int)entryRect.height, BLUE);
        }
    }

    float buttonY = y + height - 50;
    Rectangle upBtn = { x + 12, buttonY, 120, 32 };
    Rectangle selectBtn = { x + 142, buttonY, 120, 32 };
    Rectangle cancelBtn = { x + 272, buttonY, 120, 32 };

    if (GuiButton(upBtn, "Up")) {
        std::string dir(fileDialogDirectory);
        std::string parent = GetParentDirectoryLocal(dir);
        strncpy(fileDialogDirectory, parent.c_str(), sizeof(fileDialogDirectory) - 1);
        fileDialogDirectory[sizeof(fileDialogDirectory) - 1] = '\0';
        PopulateFileBrowser();
        fileDialogSelectedIndex = -1;
    }
    if (GuiButton(selectBtn, fileDialogSaveMode ? "Save" : "Open")) {
        TryLaunchSelectedBrowserEntry();
    }
    if (GuiButton(cancelBtn, "Cancel")) {
        CloseFileBrowser();
    }
}
