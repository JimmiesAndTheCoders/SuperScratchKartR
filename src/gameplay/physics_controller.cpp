#include "physics_controller.h"
#include <math.h>

PhysicsController::PhysicsController(MovementSettings settings) : config(settings) {}

void PhysicsController::ProcessMovement(Vector3& velocity, float& rotation, 
                                      float turnInput, bool isAccelerating, bool isBraking, 
                                      bool isDrifting, bool isGrounded, float grassFactor, float dt) {
    Vector3 noseForward = { sinf(rotation * DEG2RAD), 0.0f, cosf(rotation * DEG2RAD) };
    
    float speedMag = Vector3Length(velocity);
    Vector3 travelDir = (speedMag > 0.1f) ? Vector3Normalize(velocity) : noseForward;

    float driftTurnMult = isDrifting ? 1.8f : 1.0f;
    float currentForwardSpeed = Vector3DotProduct(velocity, noseForward);
    float turnSpeedFactor = (fabsf(currentForwardSpeed) / config.maxSpeed) + 0.35f;
    
    rotation += turnInput * config.turnSpeed * driftTurnMult * turnSpeedFactor * dt;

    noseForward = { sinf(rotation * DEG2RAD), 0.0f, cosf(rotation * DEG2RAD) };
    Vector3 pushDir = isDrifting ? Vector3Lerp(travelDir, noseForward, 0.3f) : noseForward;

    if (isAccelerating) {
        velocity = Vector3Add(velocity, Vector3Scale(pushDir, config.acceleration * grassFactor * dt));
    } 
    else if (isBraking) {
        if (currentForwardSpeed > 1.0f) {
            float newMag = fmaxf(0, speedMag - config.brakeForce * dt);
            velocity = Vector3Scale(travelDir, newMag);
        } else if (currentForwardSpeed > -config.reverseMaxSpeed) {
            velocity = Vector3Add(velocity, Vector3Scale(noseForward, -config.acceleration * dt));
        }
    } 
    else {
        if (speedMag > 0.1f) {
            velocity = Vector3Scale(travelDir, fmaxf(0, speedMag - config.friction * dt));
        } else {
            velocity = {0, 0, 0};
        }
    }

    if (isGrounded && speedMag > 0.1f) {
        float grip = isDrifting ? 1.2f : 7.5f; 
        if (grassFactor < 1.0f) grip *= 0.4f;

        Vector3 newTravelDir = Vector3Lerp(travelDir, noseForward, grip * dt);
        velocity = Vector3Scale(Vector3Normalize(newTravelDir), Vector3Length(velocity));
    }

    float finalMag = Vector3Length(velocity);
    float activeMax = (Vector3DotProduct(velocity, noseForward) >= 0) ? (config.maxSpeed * grassFactor) : config.reverseMaxSpeed;
    if (finalMag > activeMax) {
        velocity = Vector3Scale(Vector3Normalize(velocity), activeMax);
    }
}