#ifndef PHYSICS_CONTROLLER_H
#define PHYSICS_CONTROLLER_H

#include "raylib.h"
#include "raymath.h"

struct MovementSettings {
    float maxSpeed;
    float acceleration;
    float turnSpeed;
    float friction;
    float brakeForce;
    float reverseMaxSpeed;
};

class PhysicsController {
public:
    PhysicsController(MovementSettings settings);
    
    void ProcessMovement(Vector3& velocity, float& rotation, 
                         float turnInput, bool isAccelerating, bool isBraking, 
                         bool isDrifting, bool isGrounded, float grassFactor, float dt);

private:
    MovementSettings config;
};

#endif