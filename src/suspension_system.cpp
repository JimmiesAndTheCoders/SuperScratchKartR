#include "suspension_system.h"
#include "track.h"

SuspensionSystem::SuspensionSystem() {
    float w = 0.75f; 
    float l = 0.95f; 
    
    wheels[0].localOffset = {  w, 0.0f,  l };
    wheels[1].localOffset = { -w, 0.0f,  l };
    wheels[2].localOffset = {  w, 0.0f, -l };
    wheels[3].localOffset = { -w, 0.0f, -l };

    for (int i = 0; i < 4; i++) {
        wheels[i].suspensionLength = restLength;
        wheels[i].lastLength = restLength;
        wheels[i].isGrounded = false;
    }
}

Vector3 SuspensionSystem::Update(const Vector3& kartPos, const Quaternion& qRot, const Vector3& kartVelocity, const Track* track, float dt) {
    Vector3 totalSpringForce = { 0, 0, 0 };
    if (dt <= 0) return totalSpringForce;

    const float k_stiffness = 100.0f; 
    const float k_damping = 100.0f;     
    const float k_rideHeight = 0.45f;
    for (int i = 0; i < 4; i++) {
        Vector3 worldOffset = Vector3RotateByQuaternion(wheels[i].localOffset, qRot);
        Vector3 rayOrigin = Vector3Add(kartPos, worldOffset);
        
        float groundHeight = 0.0f;
        SurfaceType surface;
        
        wheels[i].isGrounded = false;
        if (track && track->GetGroundInfo(rayOrigin, groundHeight, surface)) {
            float distToGround = rayOrigin.y - groundHeight;
            if (distToGround <= restLength) {
                wheels[i].isGrounded = true;
                wheels[i].suspensionLength = distToGround;
                float compression = (restLength - k_rideHeight) - wheels[i].suspensionLength;
                float springVelocity = (wheels[i].lastLength - wheels[i].suspensionLength) / dt;
                float forceMag = (compression * k_stiffness) + (springVelocity * k_damping);
                if (forceMag < 0) forceMag = 0;
                totalSpringForce.y += forceMag;
            }
        }

        if (!wheels[i].isGrounded) {
            wheels[i].suspensionLength = restLength;
        }
        wheels[i].lastLength = wheels[i].suspensionLength;
    }
    return Vector3Scale(totalSpringForce, 0.25f); 
}

bool SuspensionSystem::IsAnyWheelGrounded() const {
    return wheels[0].isGrounded || wheels[1].isGrounded || wheels[2].isGrounded || wheels[3].isGrounded;
}