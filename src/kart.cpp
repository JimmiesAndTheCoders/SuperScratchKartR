#include "kart.h"
#include "raymath.h"
#include <math.h>

Kart::Kart() : position({0,0,0}), rotation(0), currentSpeed(0) {
    // Creating the mesh for the body
    Mesh mesh = GenMeshCube(1.8f, 0.8f, 3.0f);
    bodyModel = LoadModelFromMesh(mesh);
    bodyModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RED;
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

    // Controls
    if (IsKeyDown(KEY_LEFT)) rotation += config.turnSpeed * dt;
    if (IsKeyDown(KEY_RIGHT)) rotation -= config.turnSpeed * dt;

    if (IsKeyDown(KEY_UP)) {
        currentSpeed += config.acceleration * dt;
    } else if (IsKeyDown(KEY_DOWN)) {
        currentSpeed -= config.acceleration * dt;
    } else {
        if (currentSpeed > 0) currentSpeed -= config.friction * dt;
        else if (currentSpeed < 0) currentSpeed += config.friction * dt;
        if (fabsf(currentSpeed) < 0.5f) currentSpeed = 0;
    }

    // Speed caps
    if (currentSpeed > config.maxSpeed) currentSpeed = config.maxSpeed;
    if (currentSpeed < -config.maxSpeed / 3) currentSpeed = -config.maxSpeed / 3;

    // Movement
    position.x += sinf(rotation * DEG2RAD) * currentSpeed * dt;
    position.z += cosf(rotation * DEG2RAD) * currentSpeed * dt;
}

void Kart::Draw() const {
    // Body
    DrawModelEx(bodyModel, position, (Vector3){0, 1, 0}, rotation, (Vector3){1, 1, 1}, WHITE);
    
    // Aesthetic components (Wheels/Nose)
    float s = sinf(rotation * DEG2RAD);
    float c = cosf(rotation * DEG2RAD);
    float s90 = sinf((rotation + 90) * DEG2RAD);
    float c90 = cosf((rotation + 90) * DEG2RAD);

    // Front Nose
    DrawSphere({position.x + s*1.8f, position.y + 0.2f, position.z + c*1.8f}, 0.5f, YELLOW);

    // Wheels
    float wheelOffsets[4][2] = {{1.2, 1.2}, {1.2, -1.2}, {-1.0, 1.2}, {-1.0, -1.2}};
    for(int i = 0; i < 4; i++) {
        Vector3 wPos = {
            position.x + (s * wheelOffsets[i][0]) + (s90 * wheelOffsets[i][1]),
            position.y - 0.2f,
            position.z + (c * wheelOffsets[i][0]) + (c90 * wheelOffsets[i][1])
        };
        DrawSphere(wPos, 0.45f, BLACK);
    }
}