#include "powerup_manager.h"
#include "raymath.h"
#include <cstdlib>
#include <iostream>

PowerUpManager::PowerUpManager() {
    capsuleModel = {0};
}

PowerUpManager::~PowerUpManager() {
    if (capsuleModel.meshCount > 0) UnloadModel(capsuleModel);
}

void PowerUpManager::LoadModel() {
    const char* path = "assets/models/decorations/items/itemcapsule.glb";
    if (FileExists(path)) {
        capsuleModel = ::LoadModel(path);
        
        if (capsuleModel.meshCount == 0) {
            UnloadModel(capsuleModel);
            capsuleModel = {0};
            std::cout << "[PowerUpManager] failed to load capsule model\n";
        }
    } else {
        std::cout << "[PowerUpManager] capsule model not found: " << path << "\n";
    }
}

PowerUpSlot PowerUpManager::RandomPowerUp() const {
    int r = GetRandomValue(0, 2);
    switch (r) {
        case 0: return PowerUpSlot::BOOST;
        case 1: return PowerUpSlot::PROJECTILE;
        case 2: return PowerUpSlot::OBSTACLE;
        default: return PowerUpSlot::NONE;
    }
}

void PowerUpManager::GenerateCapsules(const Track* track, int count, float areaSize) {
    capsules.clear();
    if (!track) return;

    int placed = 0;
    int maxAttempts = count * 5;
    for (int i = 0; i < maxAttempts && placed < count; i++) {
        float x = (float)GetRandomValue(-(int)areaSize, (int)areaSize);
        float z = (float)GetRandomValue(-(int)areaSize, (int)areaSize);
        Vector3 testPos = { x, 50.0f, z };
        float groundHeight;
        SurfaceType type;
        if (track->GetGroundInfo(testPos, groundHeight, type)) {
            if (type == SurfaceType::ROAD) {
                bool tooClose = false;
                for (auto &c : capsules) {
                    if (Vector3Distance(c.position, {x, groundHeight, z}) < 5.0f) {
                        tooClose = true;
                        break;
                    }
                }
                if (tooClose) continue;

                CapsuleInstance inst;
                inst.position = { x, groundHeight + 0.5f, z };
                inst.scale = 1.0f;
                inst.rotation = (float)GetRandomValue(0, 360);
                inst.type = RandomPowerUp();
                inst.active = true;
                capsules.push_back(inst);
                placed++;
            }
        }
    }
    std::cout << "PowerUpManager: placed " << placed << " capsules\n";
}

void PowerUpManager::Update(Kart* player) {
    if (!player) return;
    Vector3 kartPos = player->GetPosition();
    for (auto &c : capsules) {
        if (!c.active) continue;
        float d = Vector3Distance(kartPos, c.position);
        if (d < 2.0f) {
            c.active = false;
            player->SetPowerUp(c.type);
            std::cout << "Picked up power-up\n";
        }
    }
}

void PowerUpManager::Draw() const {
    for (const auto &c : capsules) {
        if (!c.active) continue;
        if (capsuleModel.meshCount > 0) {
            DrawModelEx(capsuleModel, c.position, {0,1,0}, c.rotation, {c.scale, c.scale, c.scale}, WHITE);
        } else {
            DrawCylinderEx(c.position, Vector3Add(c.position,{0,1.5f,0}), 0.5f, 0.5f, 8, GOLD);
        }
    }
}