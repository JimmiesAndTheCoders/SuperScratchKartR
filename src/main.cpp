#include <raylib.h>

int main(void) {
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Super Scratch Kart R");

    // Camera definition
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector
    camera.fovy = 45.0f;                                // Camera field of view
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };        // Cube position

    SetTargetFPS(60);                                   // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) {                        // Detect window close button or ESC key
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                // Draw the ground grid
                DrawGrid(20, 1.0f);

                // Draw the cube
                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
            EndMode3D();

            DrawText("Super Scratch Kart R - devbuild0", 10, 10, 20, DARKGRAY);
            DrawFPS(10, 40);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}