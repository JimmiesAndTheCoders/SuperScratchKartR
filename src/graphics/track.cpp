#include "track.h"
#include "raymath.h"

Track::Track(const char* modelPath) {
    trackModel = LoadModel(modelPath);
    safetyFloorHeight = -0.1f; 
}

Track::~Track() {
    if (trackModel.meshCount > 0) UnloadModel(trackModel);
}

void Track::Draw() const {
    DrawPlane((Vector3){ 0, safetyFloorHeight, 0 }, (Vector2){ 2000, 2000 }, DARKGREEN);
    DrawModel(trackModel, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
}

bool Track::GetGroundInfo(Vector3 position, float& outHeight, SurfaceType& outType) const {
    Ray ray = { (Vector3){ position.x, position.y + 50.0f, position.z }, (Vector3){ 0.0f, -1.0f, 0.0f } };
    
    RayCollision closestHit = { 0 };
    closestHit.distance = 100000.0f;
    bool hitMesh = false;

    for (int i = 0; i < trackModel.meshCount; i++) {
        RayCollision hit = GetRayCollisionMesh(ray, trackModel.meshes[i], trackModel.transform);
        if (hit.hit && hit.distance < closestHit.distance) {
            closestHit = hit;
            hitMesh = true;
        }
    }

    if (hitMesh) {
        outHeight = closestHit.point.y;
        outType = SurfaceType::ROAD;
        return true;
    }

    outHeight = safetyFloorHeight;
    outType = SurfaceType::GRASS;
    return true;
}