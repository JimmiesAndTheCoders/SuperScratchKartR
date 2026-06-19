#include "level_editor_scene.h"
#include "level_editor.h"

#include <cmath>

LevelEditorScene::LevelEditorScene()
    : showGrid(true), cameraOffset({0.0f, 0.0f}), zoom(1.0f) {
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.position = (Vector3){0.0f, 20.0f, 40.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    desiredCameraPos = camera.position;
    cameraOffsetTarget = cameraOffset;
}

void LevelEditorScene::Update(bool gridEnabled, bool isMouseOverEditor) {
    showGrid = gridEnabled;
    if (!isMouseOverEditor) return;

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        // invert yaw/pitch signs for natural feel and apply sensitivity
        float yaw = -delta.x * orbitSensitivity;
        float pitch = -delta.y * pitchSensitivity; // inverted to fix up/down swap

        Vector3 dir = Vector3Subtract(desiredCameraPos, camera.target);
        // rotate around Y (yaw)
        float cosY = cosf(yaw);
        float sinY = sinf(yaw);
        float x = dir.x * cosY - dir.z * sinY;
        float z = dir.x * sinY + dir.z * cosY;
        dir.x = x; dir.z = z;
        // vertical movement
        dir.y += pitch * 10.0f; // more responsive vertical movement
        desiredCameraPos = Vector3Add(camera.target, dir);
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        Vector3 dir = Vector3Subtract(desiredCameraPos, camera.target);
        float len = Vector3Length(dir);
        len -= wheel * zoomSensitivity;
        if (len < 2.0f) len = 2.0f;
        Vector3 norm = Vector3Normalize(dir);
        desiredCameraPos = Vector3Add(camera.target, Vector3Scale(norm, len));
    }

    const float keyboardPan = 10.0f / zoom;
    if (IsKeyDown(KEY_RIGHT)) cameraOffsetTarget.x -= keyboardPan;
    if (IsKeyDown(KEY_LEFT)) cameraOffsetTarget.x += keyboardPan;
    if (IsKeyDown(KEY_DOWN)) cameraOffsetTarget.y -= keyboardPan;
    if (IsKeyDown(KEY_UP)) cameraOffsetTarget.y += keyboardPan;

    // smooth pan interpolation
    cameraOffset.x = cameraOffset.x + (cameraOffsetTarget.x - cameraOffset.x) * smoothing;
    cameraOffset.y = cameraOffset.y + (cameraOffsetTarget.y - cameraOffset.y) * smoothing;

    // smooth camera position interpolation
    camera.position.x = camera.position.x + (desiredCameraPos.x - camera.position.x) * smoothing;
    camera.position.y = camera.position.y + (desiredCameraPos.y - camera.position.y) * smoothing;
    camera.position.z = camera.position.z + (desiredCameraPos.z - camera.position.z) * smoothing;
}

void LevelEditorScene::Draw(int topOffset) {
    int width = GetScreenWidth();
    int height = GetScreenHeight() - topOffset;

    DrawRectangle(0, topOffset, width, height, RAYWHITE);

    BeginMode3D(camera);
    if (editor && editor->GetTrack()) {
        editor->GetTrack()->Draw();
    }
    if (editor) {
        editor->GetPowerManager().Draw();
    }
    if (showGrid) DrawGrid(100, 1.0f);
    EndMode3D();
}
