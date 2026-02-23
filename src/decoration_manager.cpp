#include "decoration_manager.h"
#include "raymath.h"
#include <iostream>

DecorationManager::DecorationManager() {}

DecorationManager::~DecorationManager() {
    for (auto& model : treeModels) {
        if (model.meshCount > 0) UnloadModel(model);
    }
}

void DecorationManager::LoadTreeModels() {
    const char* folderPath = "assets/models/decorations/trees";
    
    if (!DirectoryExists(folderPath)) {
        std::cout << "DECO: Directory not found: " << folderPath << std::endl;
        return;
    }

    FilePathList files = LoadDirectoryFiles(folderPath);

    for (unsigned int i = 0; i < files.count; i++) {
        if (IsFileExtension(files.paths[i], ".glb")) {
            Model m = LoadModel(files.paths[i]);
            if (m.meshCount > 0) {
                treeModels.push_back(m);
            } else {
                UnloadModel(m);
            }
        }
    }

    UnloadDirectoryFiles(files);
}

void DecorationManager::GenerateScatteredTrees(const Track* track, int count, float areaSize) {
    instances.clear();
    int placed = 0;
    int attempts = 0;
    
    const float minDistanceToRoad = 25.0f; 

    while (placed < count && attempts < count * 30) {
        attempts++;

        float x = (float)GetRandomValue(-(int)areaSize, (int)areaSize);
        float z = (float)GetRandomValue(-(int)areaSize, (int)areaSize);
        
        Vector3 testPos = { x, 50.0f, z };
        float groundHeight = 0.0f;
        SurfaceType type = SurfaceType::NONE;

        if (track->GetGroundInfo(testPos, groundHeight, type)) {
            if (type == SurfaceType::GRASS) {
                bool tooCloseToRoad = false;
                float step = minDistanceToRoad * 0.7f;
                Vector3 offsets[] = {
                    { step, 0, 0 }, { -step, 0, 0 }, 
                    { 0, 0, step }, { 0, 0, -step },
                    { step, 0, step }, { -step, 0, -step }
                };

                for (int i = 0; i < 6; i++) {
                    float h;
                    SurfaceType t;
                    Vector3 checkPos = Vector3Add(testPos, offsets[i]);
                    if (track->GetGroundInfo(checkPos, h, t)) {
                        if (t == SurfaceType::ROAD) {
                            tooCloseToRoad = true;
                            break;
                        }
                    }
                }

                if (!tooCloseToRoad) {
                    DecorationInstance inst;
                    inst.position = { x, groundHeight, z };
                    inst.scale = (float)GetRandomValue(630, 720) / 10.0f; 
                    inst.rotation = (float)GetRandomValue(0, 360);
                    inst.modelIndex = treeModels.empty() ? 0 : GetRandomValue(0, (int)treeModels.size() - 1);

                    instances.push_back(inst);
                    placed++;
                }
            }
        }
    }
    std::cout << "DECO: Placed " << placed << " trees with road clearance." << std::endl;
}

void DecorationManager::Draw() const {
    if (treeModels.empty()) {
        for (const auto& inst : instances) {
            DrawCylinderEx(inst.position, Vector3Add(inst.position, { 0, 2.0f * inst.scale, 0 }), 0.3f * inst.scale, 0.3f * inst.scale, 8, BROWN);
            DrawCube({ inst.position.x, inst.position.y + 3.5f * inst.scale, inst.position.z }, 2.5f * inst.scale, 3.0f * inst.scale, 2.5f * inst.scale, DARKGREEN);
        }
    } else {
        for (const auto& inst : instances) {
            DrawModelEx(treeModels[inst.modelIndex], inst.position, {0, 1, 0}, inst.rotation, {inst.scale, inst.scale, inst.scale}, WHITE);
        }
    }
}