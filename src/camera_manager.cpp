#include "camera_manager.h"
#include "raymath.h"
#include <math.h>

CameraManager::CameraManager() {
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target   = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy     = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void CameraManager::Update(Vector3 targetPos, float targetRotation) {
    float offsetX = sinf(targetRotation * DEG2RAD) * distance;
    float offsetZ = cosf(targetRotation * DEG2RAD) * distance;

    camera.position.x = targetPos.x - offsetX;
    camera.position.y = targetPos.y + height;
    camera.position.z = targetPos.z - offsetZ;

    camera.target = (Vector3){ 
        targetPos.x, 
        targetPos.y + lookAtOffset, 
        targetPos.z 
    };
}