#include "kart_model.h"
#include "asset_utils.h"
#include "math_utils.h"
#include "raymath.h"
#include "rlgl.h"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

KartModel::KartModel(const char* modelPath) {
    std::string resolvedPath = ResolveAssetPath(modelPath);
    hasValidModel = false;
    
    if (fs::exists(resolvedPath)) {
        model = LoadModel(resolvedPath.c_str());
        if (model.meshCount > 0) hasValidModel = true;
    }
    
    if (!hasValidModel) {
        Mesh box = GenMeshCube(1.5f, 0.8f, 2.5f);
        model = LoadModelFromMesh(box);
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RED;
    }
    
    animCount = 0;
    animations = LoadModelAnimations(resolvedPath.c_str(), &animCount);
    currentAnimIndex = 0;
    animFrameCounter = 0;
}

void KartModel::ApplyTexture(const char* texturePath) {
    std::string resolved = ResolveAssetPath(texturePath);
    if (fs::exists(resolved)) {
        Texture2D tex = LoadTexture(resolved.c_str());
        for (int i = 0; i < model.materialCount; i++) {
            model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = tex;
        }
    }
}

KartModel::~KartModel() {
    if (model.meshCount > 0) UnloadModel(model);
    if (animations != nullptr) UnloadModelAnimations(animations, animCount);
}

void KartModel::Update(float dt) {
    if (animCount > 0 && animations != nullptr) {
        animFrameCounter += (int)(dt * 30.0f);
        UpdateModelAnimation(model, animations[currentAnimIndex], animFrameCounter);
        if (animFrameCounter >= animations[currentAnimIndex].frameCount) animFrameCounter = 0;
    }
}

void KartModel::Draw(Vector3 position, float rotationY, float tiltZ, float squish, 
                     float wheelSpin, float steerAngle, Color tint) const {
    float scaleFactor = hasValidModel ? 2.5f : 1.0f;
    Matrix transform = MatrixIdentity();
    transform = MatrixMultiply(transform, MatrixScale(scaleFactor, scaleFactor * squish, scaleFactor));
    transform = MatrixMultiply(transform, MatrixRotateZ(tiltZ * DEG2RAD));
    transform = MatrixMultiply(transform, MatrixRotateY(rotationY * DEG2RAD));
    transform = MatrixMultiply(transform, MatrixTranslate(position.x, position.y, position.z));
    Model* m = const_cast<Model*>(&model);
    m->transform = transform;
    DrawModel(*m, {0, 0, 0}, 1.0f, tint);
}

void KartModel::SetAnimationState(KartAnimState state) {
    int targetAnimIndex = (int)state;
    if (targetAnimIndex < animCount && targetAnimIndex != currentAnimIndex) {
        currentAnimIndex = targetAnimIndex;
        animFrameCounter = 0;
    }
}