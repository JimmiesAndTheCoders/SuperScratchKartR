#ifndef KART_H
#define KART_H

#include "raylib.h"
#include "wheel.h"
#include "particle_system.h"
#include "drift_controller.h"

class Track;

struct KartPhysics {
    float acceleration = 35.0f;
    float friction = 15.0f;
    float maxSpeed = 45.0f;
    float turnSpeed = 130.0f;
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
    float GetSpeed() const; 
    bool IsDrifting() const { return isDrifting; }

private:
    Vector3 position;
    Vector3 velocity; // Replaced currentSpeed with a robust velocity vector
    float rotation;
    float currentSteerAngle;
    float velocityY;
    
    bool isDrifting;

    KartPhysics config;
    Model bodyModel;
    DriftController drifter;
    Wheel wheels[4];
    
    ParticleSystem dustParticles; // Added our new particle system
};

#endif