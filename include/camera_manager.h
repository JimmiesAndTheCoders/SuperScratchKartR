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
    
    // Stored vectors for smoothing
    Vector3 currentCamPos;
    Vector3 currentLookAt;

    // Camera settings
    const float distance = 12.0f;
    const float height = 5.0f;
    const float lookAtOffset = 2.0f;
    
    // Smoothing factor (Higher = faster follow, Lower = lazier/smoother)
    const float followLerpSpeed = 8.0f;
    const float lookAtLerpSpeed = 12.0f;
};

#endif