#include "kart.h"
#include "track.h"
#include "raymath.h"
#include <math.h>

Kart::Kart() : position({0, 0, 0}), rotation(0), currentSpeed(0), currentSteerAngle(0), velocityY(0) {
    Mesh mesh = GenMeshCube(1.8f, 0.8f, 3.0f);
    bodyModel = LoadModelFromMesh(mesh);
    bodyModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RED;

    wheels[0] = Wheel({ 0.9f, -0.2f, 1.2f }, 0.5f, true);
    wheels[1] = Wheel({ -0.9f, -0.2f, 1.2f }, 0.5f, true);
    wheels[2] = Wheel({ 0.9f, -0.2f, -1.0f }, 0.5f, false);
    wheels[3] = Wheel({ -0.9f, -0.2f, -1.0f }, 0.5f, false);
}

Kart::Kart(Vector3 startPos) : Kart() {
    position = startPos;
}

Kart::~Kart() {
    if (bodyModel.meshCount > 0) UnloadModel(bodyModel);
}

void Kart::Update(const Track* currentTrack) {
    float dt = GetFrameTime();
    SurfaceType currentSurface = SurfaceType::NONE;
    float groundHeight = -100.0f;

    // Get track info
    if (currentTrack) {
        currentTrack->GetGroundInfo(position, groundHeight, currentSurface);
    }

    // Determine physics modifiers based on surface
    float speedMultiplier = 1.0f;
    float accelMultiplier = 1.0f;

    if (currentSurface == SurfaceType::GRASS) {
        speedMultiplier = 0.3f;  // 70% slower top speed on grass
        accelMultiplier = 0.5f;  // Slower acceleration
    }

    // Input
    bool moveLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    bool moveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool moveUp = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    bool moveDown = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);

    // Turning
    if (moveLeft) rotation += config.turnSpeed * dt;
    if (moveRight) rotation -= config.turnSpeed * dt;

    // Acceleration with Grass Penalty
    float targetMaxSpeed = config.maxSpeed * speedMultiplier;
    
    if (moveUp) {
        currentSpeed += config.acceleration * accelMultiplier * dt;
    } else if (moveDown) {
        currentSpeed -= config.acceleration * dt;
    } else {
        // Natural friction
        if (currentSpeed > 0) currentSpeed -= config.friction * dt;
        else if (currentSpeed < 0) currentSpeed += config.friction * dt;
        if (fabsf(currentSpeed) < 0.5f) currentSpeed = 0;
    }

    // Apply speed cap (Dynamic based on surface)
    if (currentSpeed > targetMaxSpeed) {
        // If we just entered grass at high speed, bleed speed off quickly
        currentSpeed = Lerp(currentSpeed, targetMaxSpeed, 5.0f * dt);
    }
    if (currentSpeed < -targetMaxSpeed / 3) currentSpeed = -targetMaxSpeed / 3;

    // Gravity
    velocityY -= 30.0f * dt;
    position.y += velocityY * dt;

    // Movement
    position.x += sinf(rotation * DEG2RAD) * currentSpeed * dt;
    position.z += cosf(rotation * DEG2RAD) * currentSpeed * dt;

    // Ground Snap
    float kartHeightOffset = 0.4f;
    if (position.y - kartHeightOffset < groundHeight) {
        position.y = groundHeight + kartHeightOffset;
        velocityY = 0.0f;
    }
}

void Kart::Draw() const {
    DrawModelEx(bodyModel, position, (Vector3){0, 1, 0}, rotation, (Vector3){1, 1, 1}, WHITE);
    for(int i = 0; i < 4; i++) wheels[i].Draw(position, rotation, currentSteerAngle, 0);
}