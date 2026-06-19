#include "camera_manager.h"
#include "raymath.h"
#include <math.h>

CameraManager::CameraManager() {
    camera = { 0 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Initialize smoothing vectors to avoid starting at (0,0,0)
    currentCamPos = (Vector3){ 0.0f, 10.0f, 10.0f };
    currentLookAt = (Vector3){ 0.0f, 0.0f, 0.0f };
}

void CameraManager::Update(Vector3 targetPos, float targetRotation) {
    float dt = GetFrameTime();

    // 1. Calculate the IDEAL position (where the camera WANTS to be)
    float offsetX = sinf(targetRotation * DEG2RAD) * distance;
    float offsetZ = cosf(targetRotation * DEG2RAD) * distance;

    Vector3 idealPos = {
        targetPos.x - offsetX,
        targetPos.y + height,
        targetPos.z - offsetZ
    };

    // 2. Calculate the IDEAL look-at point
    Vector3 idealLookAt = {
        targetPos.x,
        targetPos.y + lookAtOffset,
        targetPos.z
    };

    // 3. LERP from current state to ideal state
    // Vector3Lerp(start, end, amount)
    currentCamPos = Vector3Lerp(currentCamPos, idealPos, followLerpSpeed * dt);
    currentLookAt = Vector3Lerp(currentLookAt, idealLookAt, lookAtLerpSpeed * dt);

    // 4. Apply to the actual Raylib camera
    camera.position = currentCamPos;
    camera.target = currentLookAt;
}