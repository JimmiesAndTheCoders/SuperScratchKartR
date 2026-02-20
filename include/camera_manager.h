#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "raylib.h"

class CameraManager {
public:
    CameraManager();
    void Update(Vector3 targetPos, float targetRotation);
    Camera3D GetInternalCamera() const { return camera; }

private:
    Camera3D camera;
    
    const float distance = 12.0f;
    const float height = 5.0f;
    const float lookAtOffset = 2.0f;
};

#endif