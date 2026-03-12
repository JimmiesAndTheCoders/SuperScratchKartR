#ifndef DRIFT_CONTROLLER_H
#define DRIFT_CONTROLLER_H

#include "raylib.h"

enum DriftDirection { DRIFT_NONE, DRIFT_LEFT, DRIFT_RIGHT };

class DriftController {
public:
    DriftController();

    // Logic updates
    void Update(bool driftKey, float turnInput, bool isGrounded, float speed, float dt);
    
    // Getters for the Kart to use
    bool IsDrifting() const { return direction != DRIFT_NONE; }
    float GetGripModifier() const;
    float GetTurnModifier() const;
    float GetVisualTilt() const { return visualTilt; }
    float GetHopVelocity() const { return hopRequest ? 3.5f : 0.0f; }
    
    // Reset hop after Kart processes it
    void ConsumeHop() { hopRequest = false; }

private:
    DriftDirection direction;
    float visualTilt;
    bool hopRequest;
    bool prevDriftKey;
};

#endif