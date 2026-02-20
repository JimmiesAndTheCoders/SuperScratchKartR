#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "raylib.h"

class CameraManager {
public:
    CameraManager();
    void Update(Vector3 target);
    Camera3D GetInternalCamera() const { return camera; }

private:
    Camera3D camera;
};

#endif