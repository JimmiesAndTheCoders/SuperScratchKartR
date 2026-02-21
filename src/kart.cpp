#include "kart.h"
#include "track.h"
#include "raymath.h"
#include <math.h>

Kart::Kart() : position({0, 0, 0}), velocity({0, 0, 0}), rotation(0), 
               currentSpeed(0), currentSteerAngle(0), velocityY(0), 
               isDrifting(false), driftDirection(0) {
    Mesh mesh = GenMeshCube(1.8f, 0.8f, 3.0f);
    bodyModel = LoadModelFromMesh(mesh);
    bodyModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RED;

    wheels[0] = Wheel({ 0.9f, -0.2f, 1.2f }, 0.5f, true);
    wheels[1] = Wheel({ -0.9f, -0.2f, 1.2f }, 0.5f, true);
    wheels[2] = Wheel({ 0.9f, -0.2f, -1.0f }, 0.5f, false);
    wheels[3] = Wheel({ -0.9f, -0.2f, -1.0f }, 0.5f, false);
    
    config.turnSpeed = 100.0f; 
    config.driftTurnBoost = 1.3f; 
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

    if (currentTrack) {
        currentTrack->GetGroundInfo(position, groundHeight, currentSurface);
    }

    float speedMultiplier = (currentSurface == SurfaceType::GRASS) ? 0.35f : 1.0f;

    // Input
    bool moveLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    bool moveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool moveUp = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    bool moveDown = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    bool driftKey = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_SPACE);

    // Drifting Toggle Logic
    if (driftKey && !isDrifting && currentSpeed > (config.maxSpeed * 0.4f)) {
        if (moveLeft) { isDrifting = true; driftDirection = 1; velocityY = 3.5f; }
        else if (moveRight) { isDrifting = true; driftDirection = -1; velocityY = 3.5f; }
    }

    if (!driftKey) isDrifting = false;

    // Physics: Handling Rotation
    if (isDrifting) {
        float steerInput = (float)moveLeft - (float)moveRight;
        // The nose of the kart points sharply into the turn
        rotation += (driftDirection * config.turnSpeed * config.driftTurnBoost * dt);
        // User can slightly influence the angle
        rotation += (steerInput * config.turnSpeed * 0.4f * dt);
    } else {
        if (moveLeft) rotation += config.turnSpeed * dt;
        if (moveRight) rotation -= config.turnSpeed * dt;
    }

    // Physics: Speed
    float targetMaxSpeed = config.maxSpeed * speedMultiplier;
    if (moveUp) currentSpeed += config.acceleration * dt;
    else if (moveDown) currentSpeed -= config.acceleration * dt;
    else currentSpeed = Lerp(currentSpeed, 0, 2.0f * dt);

    currentSpeed = Clamp(currentSpeed, -targetMaxSpeed/3, targetMaxSpeed);

    // Physics: Movement Vector (The "Slide")
    Vector3 forwardDir = { sinf(rotation * DEG2RAD), 0, cosf(rotation * DEG2RAD) };
    
    // Low friction while drifting means velocity doesn't align with rotation immediately
    float grip = isDrifting ? 1.4f : 8.0f; 
    
    velocity.x = Lerp(velocity.x, forwardDir.x * currentSpeed, grip * dt);
    velocity.z = Lerp(velocity.z, forwardDir.z * currentSpeed, grip * dt);

    // Gravity & Position
    velocityY -= 25.0f * dt;
    position.x += velocity.x * dt;
    position.z += velocity.z * dt;
    position.y += velocityY * dt;

    // Ground Collision
    if (position.y < groundHeight + 0.4f) {
        position.y = groundHeight + 0.4f;
        velocityY = 0;
    }

    // Wheel visual steering
    float targetSteer = (moveLeft ? 25.0f : (moveRight ? -25.0f : 0));
    currentSteerAngle = Lerp(currentSteerAngle, targetSteer, 10.0f * dt);
}

void Kart::Draw() const {
    // 1. Calculate Tilt Angle
    float tiltAngle = isDrifting ? (driftDirection * 7.0f) : 0;

    // 2. Build the transformation matrix for DRAWING (don't save to bodyModel)
    Matrix matRotation = MatrixRotateY(rotation * DEG2RAD);
    Matrix matTilt = MatrixRotateZ(tiltAngle * DEG2RAD);
    Matrix matTransform = MatrixMultiply(matTilt, matRotation);
    matTransform = MatrixMultiply(matTransform, MatrixTranslate(position.x, position.y, position.z));

    // 3. Draw using the temporary matrix
    // We use the first mesh of the model to draw with the calculated transform
    DrawMesh(bodyModel.meshes[0], bodyModel.materials[0], matTransform);
    
    // 4. Render wheels
    for(int i = 0; i < 4; i++) {
        wheels[i].Draw(position, rotation, currentSteerAngle, 0);
    }
}