#include "kart.h"
#include "track.h"
#include "physics_controller.h"
#include <math.h>

struct KartInputState {
    float turn;
    bool accelerating;
    bool braking;
    bool driftHolding;
    bool hopPressed;
};

static KartInputState GetKartInput() {
    KartInputState input = {0};
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) input.turn += 1.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) input.turn -= 1.0f;
    input.accelerating = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    input.braking = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    input.driftHolding = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    input.hopPressed = IsKeyPressed(KEY_Z); 
    return input;
}

Kart::Kart(const char* modelPath) 
    : position({0, 0, 0}), velocity({0, 0, 0}), rotation(0), 
      velocityY(0), currentGroundHeight(0), isDrifting(false), 
      wheelSpin(0.0f), visualModel(modelPath) 
{
    MovementSettings settings;
    settings.maxSpeed = 55.0f;
    settings.acceleration = 28.0f;
    settings.turnSpeed = 135.0f;
    settings.friction = 8.0f;
    settings.brakeForce = 200.0f;
    settings.reverseMaxSpeed = 25.0f;
    
    physics = new PhysicsController(settings);
    visualModel.ApplyTexture("assets/images/textures/colormap.png");
    
    juice.tilt = 0.0f;
    juice.squish = 1.0f;
    juice.visualOffset = {0,0,0};
}

Kart::Kart(Vector3 startPos, const char* modelPath) : Kart(modelPath) {
    position = startPos;
}

Kart::~Kart() {
    delete physics;
}

float Kart::GetSpeed() const {
    Vector3 forward = { sinf(rotation * DEG2RAD), 0.0f, cosf(rotation * DEG2RAD) };
    return Vector3DotProduct(velocity, forward);
}

void Kart::Update(const Track* currentTrack) {
    float dt = GetFrameTime();
    if (dt > 0.05f) dt = 0.0166f; 
    SurfaceType surface = SurfaceType::ROAD;
    if (currentTrack) currentTrack->GetGroundInfo(position, currentGroundHeight, surface);
    static bool isInitialized = false;
    if (!isInitialized && currentTrack) {
        position.y = currentGroundHeight;
        velocityY = 0;
        Quaternion qRot = QuaternionFromEuler(0, rotation * DEG2RAD, 0);
        suspension.Update(position, qRot, velocity, currentTrack, 0.016f);
        isInitialized = true;
    }

    KartInputState input = GetKartInput();
    float grassFactor = (surface == SurfaceType::GRASS) ? 0.4f : 1.0f;
    
    Quaternion qRot = QuaternionFromEuler(0, rotation * DEG2RAD, 0);
    Vector3 suspForce = suspension.Update(position, qRot, velocity, currentTrack, dt);
    bool isGrounded = suspension.IsAnyWheelGrounded();
    
    float speed = GetSpeed();
    float effectiveTurn = input.turn;
    if (speed < -0.1f) effectiveTurn = -input.turn;

    if (input.driftHolding && isGrounded && speed > 10.0f && fabsf(input.turn) > 0.1f) {
        isDrifting = true;
    } else if (!input.driftHolding) {
        isDrifting = false;
    }
    physics->ProcessMovement(velocity, rotation, effectiveTurn, 
                             input.accelerating, input.braking, 
                             isDrifting, isGrounded, grassFactor, dt);
    if (isDrifting && isGrounded) {
        float driftSlowdown = 0.85f;
        velocity = Vector3Scale(velocity, 1.0f - (1.0f - driftSlowdown) * dt * 5.0f);
    }
    float newSpeed = GetSpeed();
    velocity.x = sinf(rotation * DEG2RAD) * newSpeed;
    velocity.z = cosf(rotation * DEG2RAD) * newSpeed;
    position.x += velocity.x * dt;
    position.z += velocity.z * dt;
    if (isGrounded) {
        float targetY = currentGroundHeight + 0.05f;
        float netVerticalForce = suspForce.y - 15.0f; 
        velocityY += netVerticalForce * dt;
        velocityY *= 0.9f; 
        position.y += velocityY * dt;
        if (position.y < currentGroundHeight) {
            position.y = currentGroundHeight;
            if (velocityY < 0) velocityY = 0;
        }

        if (input.hopPressed) {
            velocityY = 12.0f; 
            juice.squish = 0.6f;
        }
    } else {
        velocityY -= 35.0f * dt; 
        position.y += velocityY * dt;
        
        if (position.y < currentGroundHeight) {
            position.y = currentGroundHeight;
            velocityY = 0;
        }
    }

    wheelSpin += newSpeed * 5.0f * dt;
    juice.squish = Lerp(juice.squish, 1.0f, 15.0f * dt);
    juice.tilt = Lerp(juice.tilt, input.turn * -5.0f, 5.0f * dt);

    if (isGrounded && (grassFactor < 1.0f || isDrifting) && Vector3Length(velocity) > 10.0f) {
        Vector3 forward = { sinf(rotation * DEG2RAD), 0.0f, cosf(rotation * DEG2RAD) };
        Vector3 right = { cosf(rotation * DEG2RAD), 0.0f, -sinf(rotation * DEG2RAD) };
        Vector3 backPos = Vector3Subtract(position, Vector3Scale(forward, 1.2f));
        dustParticles.EmitDust(Vector3Add(backPos, Vector3Scale(right, 0.6f)), (grassFactor < 1.0f));
        dustParticles.EmitDust(Vector3Add(backPos, Vector3Scale(right, -0.6f)), (grassFactor < 1.0f));
    }
    dustParticles.Update(dt);
}

void Kart::Draw() const {
    blobShadow.Draw({position.x, currentGroundHeight + 0.02f, position.z}, 1.8f, 0.4f);
    float steerVisual = 0.0f;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) steerVisual = 25.0f;
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) steerVisual = -25.0f;
    visualModel.Draw(position, rotation, juice.tilt, juice.squish, wheelSpin, steerVisual, WHITE);
    dustParticles.Draw();
}