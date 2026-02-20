#include "renderer.h"

Renderer::Renderer() {
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void Renderer::UpdateCamera(Vector3 target) {
    camera.target = target;
}

void Renderer::DrawScene(Vector3 cubePos) {
    BeginMode3D(camera);
        DrawGrid(20, 1.0f);
        DrawCube(cubePos, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePos, 2.0f, 2.0f, 2.0f, MAROON);
    EndMode3D();
}

void Renderer::DrawUI() {
    DrawText("Super Scratch Kart R - devbuild0", 10, 10, 20, DARKGRAY);
    DrawFPS(10, 40);
}