#ifndef KART_H
#define KART_H

#include "raylib.h"
#include "wheel.h"

class Track;

struct KartPhysics {
    float acceleration = 35.0f;
    float friction = 15.0f;
    float maxSpeed = 45.0f;
    float turnSpeed = 130.0f;
    float driftTurnBoost = 1.5f; // Faster turning while drifting
    float gripLossThreshold = 0.6f; // Speed % required to start a drift
};

class Kart {
public:
    Kart();
    Kart(Vector3 startPos);
    ~Kart();

    void Update(const Track* currentTrack);
    void Draw() const;

    Vector3 GetPosition() const { return position; }
    float GetRotation() const { return rotation; }
    float GetSpeed() const { return currentSpeed; }
    bool IsDrifting() const { return isDrifting; }

private:
    Vector3 position;
    Vector3 velocity; // For robust physics
    float rotation;
    float currentSpeed;
    float currentSteerAngle;
    float velocityY;

    // Drift State
    bool isDrifting;
    int driftDirection; // -1 for Left, 1 for Right, 0 for None

    KartPhysics config;
    Model bodyModel;
    Wheel wheels[4];
};

#endif