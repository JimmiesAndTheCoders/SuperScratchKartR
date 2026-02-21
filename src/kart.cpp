#include "kart.h"
#include "track.h"
#include "drift_controller.h"
#include "raymath.h"
#include "rlgl.h" 
#include <math.h>

Kart::Kart() : position({0, 0, 0}), velocity({0, 0, 0}), rotation(0), currentSteerAngle(0), velocityY(0), isDrifting(false) {
    Mesh mesh = GenMeshCube(1.8f, 0.8f, 3.0f);
    bodyModel = LoadModelFromMesh(mesh);
    bodyModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = RED;

    // --- DRASTICALLY REDUCED PHYSICS VALUES ---
    // Max speed 18 is much more manageable for a kart scale.
    config.maxSpeed = 50.0f;     
    config.acceleration = 12.5f; 
    config.turnSpeed = 95.0f;    
    config.friction = 5.0f;      

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

float Kart::GetSpeed() const {
    return Vector3Length({velocity.x, 0, velocity.z}); 
}

void Kart::Update(const Track* currentTrack) {
    float dt = GetFrameTime();
    SurfaceType currentSurface = SurfaceType::NONE;
    float groundHeight = -100.0f;

    if (currentTrack) {
        currentTrack->GetGroundInfo(position, groundHeight, currentSurface);
    }

    // --- HEAVY GRASS PENALTY ---
    bool onGrass = (currentSurface == SurfaceType::GRASS);
    float grassSpeedFactor = onGrass ? 0.5f : 1.0f; // Only 50% speed on grass
    float effectiveFriction = onGrass ? 25.0f : config.friction;
    float effectiveAcceleration = onGrass ? 10.0f : config.acceleration;

    float kartHeightOffset = 0.4f;
    bool isGrounded = (position.y - kartHeightOffset <= groundHeight + 0.15f);

    // Inputs
    float turnInput = 0.0f;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) turnInput += 1.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) turnInput -= 1.0f;

    // Update Drift Controller
    drifter.Update(false, turnInput, isGrounded, GetSpeed(), dt);
    
    float hop = drifter.GetHopVelocity();
    if (hop > 0) {
        velocityY = hop * 0.8f; // Shorter hop
        drifter.ConsumeHop();
    }
    
    isDrifting = drifter.IsDrifting();

    // Turn Logic
    rotation += turnInput * (config.turnSpeed * drifter.GetTurnModifier()) * dt;

    Vector3 forward = { sinf(rotation * DEG2RAD), 0.0f, cosf(rotation * DEG2RAD) };
    Vector3 rightVec = { cosf(rotation * DEG2RAD), 0.0f, -sinf(rotation * DEG2RAD) };

    // Movement Physics
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        velocity = Vector3Add(velocity, Vector3Scale(forward, effectiveAcceleration * dt));
    } else {
        float airFriction = 1.5f;
        float currentFriction = isGrounded ? effectiveFriction : airFriction;
        float mag = GetSpeed();
        if (mag > 0.1f) {
            velocity = Vector3Scale(Vector3Normalize(velocity), mag - (currentFriction * dt));
        } else {
            velocity = {0,0,0};
        }
    }

    // Speed Cap (Smoothly forcing the speed down if over the limit)
    float targetMax = config.maxSpeed * grassSpeedFactor;
    float currentSpeed = GetSpeed();
    if (currentSpeed > targetMax) {
        float slowdownSpeed = (onGrass ? 50.0f : 15.0f); // Fast slowdown on grass
        velocity = Vector3Scale(Vector3Normalize(velocity), currentSpeed - (slowdownSpeed * dt));
    }

    // Grip/Slide Physics
    if (isGrounded && currentSpeed > 0.1f) {
        velocity = Vector3Lerp(velocity, Vector3Scale(forward, currentSpeed), drifter.GetGripModifier() * dt);
    }

    // Gravity
    velocityY -= 18.0f * dt;
    position.y += velocityY * dt;
    
    // Horizontal Movement
    position.x += velocity.x * dt;
    position.z += velocity.z * dt;

    if (position.y - kartHeightOffset < groundHeight) {
        position.y = groundHeight + kartHeightOffset;
        if (velocityY < 0) velocityY = 0; 
    }

    // Particles
    if (isDrifting && isGrounded && currentSpeed > 3.0f) {
        Vector3 backPos = Vector3Add(position, Vector3Scale(forward, -1.2f));
        dustParticles.EmitDust(Vector3Add(backPos, Vector3Scale(rightVec, 0.7f)));
        dustParticles.EmitDust(Vector3Add(backPos, Vector3Scale(rightVec, -0.7f)));
    }
    dustParticles.Update(dt);
}

void Kart::Draw() const {
    rlPushMatrix();
        rlTranslatef(position.x, position.y, position.z);
        rlRotatef(rotation, 0, 1, 0); 
        rlRotatef(drifter.GetVisualTilt(), 0, 0, 1); 
        DrawModel(bodyModel, {0,0,0}, 1.0f, WHITE);
    rlPopMatrix();

    for(int i = 0; i < 4; i++) wheels[i].Draw(position, rotation, currentSteerAngle, 0);
    dustParticles.Draw();
}