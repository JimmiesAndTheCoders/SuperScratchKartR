#include "kart.h"
#include "raymath.h"
#include <math.h>

Kart::Kart() : position({0,0,0}), rotation(0), currentSpeed(0), currentSteerAngle(0) {
    Mesh mesh = GenMeshCube(1.8f, 0.8f, 3.0f);
    bodyModel = LoadModelFromMesh(mesh);
    bodyModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RED;

    wheels[0] = Wheel({ 0.9f, -0.2f,  1.2f}, 0.5f, true);  
    wheels[1] = Wheel({-0.9f, -0.2f,  1.2f}, 0.5f, true);  
    wheels[2] = Wheel({ 0.9f, -0.2f, -1.0f}, 0.5f, false); 
    wheels[3] = Wheel({-0.9f, -0.2f, -1.0f}, 0.5f, false); 
}

Kart::Kart(Vector3 startPos) : Kart() {
    position = startPos;
}

Kart::~Kart() {
    // Unloading model safely
    if (bodyModel.meshCount > 0) {
        UnloadModel(bodyModel);
    }
}

void Kart::Update() {
    float dt = GetFrameTime();

    bool moveLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    bool moveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool moveUp = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    bool moveDown = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);

    float targetSteer = 0;
    if (moveLeft) {
        rotation += config.turnSpeed * dt;
        targetSteer = 25.0f;
    } else if (moveRight) {
        rotation -= config.turnSpeed * dt;
        targetSteer = -25.0f;
    }

    currentSteerAngle = Lerp(currentSteerAngle, targetSteer, 10.0f * dt);

    if (moveUp) currentSpeed += config.acceleration * dt;
    else if (moveDown) currentSpeed -= config.acceleration * dt;
    else {
        if (currentSpeed > 0) currentSpeed -= config.friction * dt;
        else if (currentSpeed < 0) currentSpeed += config.friction * dt;
        if (fabsf(currentSpeed) < 0.5f) currentSpeed = 0;
    }

    if (currentSpeed > config.maxSpeed) currentSpeed = config.maxSpeed;
    if (currentSpeed < -config.maxSpeed / 3) currentSpeed = -config.maxSpeed / 3;

    position.x += sinf(rotation * DEG2RAD) * currentSpeed * dt;
    position.z += cosf(rotation * DEG2RAD) * currentSpeed * dt;
}

void Kart::Draw() const {
    DrawModelEx(bodyModel, position, (Vector3){0, 1, 0}, rotation, (Vector3){1, 1, 1}, WHITE);
    
    float s = sinf(rotation * DEG2RAD);
    float c = cosf(rotation * DEG2RAD);
    DrawSphere({position.x + s*1.8f, position.y + 0.2f, position.z + c*1.8f}, 0.5f, YELLOW);

    for(int i = 0; i < 4; i++) {
        wheels[i].Draw(position, rotation, currentSteerAngle, 0);
    }
}