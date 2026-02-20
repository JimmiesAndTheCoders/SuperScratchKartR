#include "camera_manager.h"

CameraManager::CameraManager() {
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target   = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy     = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void CameraManager::Update(Vector3 target) {
    camera.target = target;
    // Coming soon...
}