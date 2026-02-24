#ifndef SUSPENSION_SYSTEM_H
#define SUSPENSION_SYSTEM_H

#include "raylib.h"
#include "raymath.h"

class Track;

struct WheelData {
    Vector3 localOffset;     
    float suspensionLength;  
    float lastLength;        
    bool isGrounded;
};

class SuspensionSystem {
public:
    SuspensionSystem();
    float restLength = 0.9f;    
    float springForce = 180.0f; 
    float damperForce = 12.0f;  
    Vector3 Update(const Vector3& kartPos, const Quaternion& qRot, const Vector3& kartVelocity, const Track* track, float dt);
    bool IsAnyWheelGrounded() const;

private:
    WheelData wheels[4]; 
};

#endif