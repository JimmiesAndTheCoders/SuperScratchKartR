#ifndef LEVEL_EDITOR_SCENE_H
#define LEVEL_EDITOR_SCENE_H

#include "raylib.h"

class LevelEditor;

class LevelEditorScene {
public:
    LevelEditorScene();
    void Update(bool gridEnabled, bool isMouseOverEditor);
    void Draw(int topOffset);
    void SetEditor(LevelEditor* e) { editor = e; }

private:
    bool showGrid;
    Vector2 cameraOffset;
    float zoom;
    Camera3D camera;
    LevelEditor* editor{nullptr};
    // smoothing and control state
    Vector3 desiredCameraPos;
    Vector2 cameraOffsetTarget;
    float orbitSensitivity = 0.02f;
    float pitchSensitivity = 0.04f;
    float zoomSensitivity = 4.0f;
    float smoothing = 0.18f;
};

#endif
