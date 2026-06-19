// Implements track import and level file load/save operations
#include "level_editor.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

bool LevelEditor::ImportTrack(const char* trackPath) {
    if (!FileExists(trackPath)) {
        std::cout << "[Editor] Track file not found: " << trackPath << std::endl;
        return false;
    }

    if (currentTrack) {
        delete currentTrack;
        currentTrack = nullptr;
    }
    currentTrack = new Track(trackPath);
    std::strncpy(currentTrackPath, trackPath, sizeof(currentTrackPath) - 1);
    currentTrackPath[sizeof(currentTrackPath) - 1] = '\0';
    powerManager.ClearCapsules();
    fileDirty = true;
    return true;
}

bool LevelEditor::LoadLevelFile(const char* filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "[Editor] File not found: " << filename << std::endl;
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }
    in.close();

    if (lines.empty()) {
        std::cout << "[Editor] Level file is empty: " << filename << std::endl;
        return false;
    }

    const std::string& trackPath = lines[0];
    if (!ImportTrack(trackPath.c_str())) {
        std::cout << "[Editor] Failed to import track from file: " << trackPath << std::endl;
        return false;
    }

    std::vector<CapsuleInstance> loadedCapsules;
    for (size_t i = 1; i < lines.size(); ++i) {
        if (lines[i].rfind("POWERUP ", 0) == 0) {
            CapsuleInstance inst = {0};
            int typeInt = 0;
            int activeInt = 0;
            std::istringstream lineStream(lines[i].substr(8));
            lineStream >> inst.position.x >> inst.position.y >> inst.position.z >> inst.rotation >> inst.scale >> typeInt >> activeInt;
            inst.type = static_cast<PowerUpSlot>(typeInt);
            inst.active = activeInt != 0;
            loadedCapsules.push_back(inst);
        }
    }

    powerManager.SetCapsules(loadedCapsules);
    std::strncpy(currentFile, filename, sizeof(currentFile) - 1);
    currentFile[sizeof(currentFile) - 1] = '\0';
    fileDirty = false;
    return true;
}

bool LevelEditor::SaveLevelFile(const char* filename) {
    if (!DirectoryExists("assets/levels")) {
        MakeDirectory("assets/levels");
    }

    if (currentTrackPath[0] == '\0') {
        std::cout << "[Editor] No track imported for save." << std::endl;
        return false;
    }

    std::string content;
    content += std::string(currentTrackPath) + "\n";
    const auto& capsules = powerManager.GetCapsules();
    for (const auto& c : capsules) {
        content += TextFormat("POWERUP %.2f %.2f %.2f %.2f %.2f %d %d\n",
            c.position.x, c.position.y, c.position.z,
            c.rotation, c.scale,
            static_cast<int>(c.type), c.active ? 1 : 0);
    }

    FILE* out = fopen(filename, "wb");
    if (!out) return false;
    fwrite(content.c_str(), 1, content.size(), out);
    fclose(out);
    return true;
}

void LevelEditor::CreateNewLevel() {
    const char* path = "assets/models/tracks/test.glb";
    if (ImportTrack(path)) {
        const char* filePath = "assets/levels/untitled.sskrt";
        std::strncpy(currentFile, filePath, sizeof(currentFile) - 1);
        currentFile[sizeof(currentFile) - 1] = '\0';
        fileDirty = true;
        std::cout << "[Editor] New level created: " << currentFile << std::endl;
    }
}
