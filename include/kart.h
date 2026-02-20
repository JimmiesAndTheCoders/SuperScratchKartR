#ifndef KART_H
#define KART_H

#include "raylib.h"
#include "wheel.h"

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
    
    void Update();
    void Draw() const;

    Vector3 GetPosition() const { return position; }
    float GetRotation() const { return rotation; }
    float GetSpeed() const { return currentSpeed; }

private:
    Vector3 position;
    float rotation;        
    float currentSpeed;
	float currentSteerAngle;    
    KartPhysics config;
    Model bodyModel;
    Wheel wheels[4];
};

#endif