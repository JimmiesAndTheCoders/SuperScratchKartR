#include "drift_controller.h"
#include <math.h>

DriftController::DriftController() 
    : direction(DRIFT_NONE), visualTilt(0.0f), hopRequest(false), prevDriftKey(false) {}

void DriftController::Update(bool driftKey, float turnInput, bool isGrounded, float speed, float dt) {
    bool shiftPressed = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    if (shiftPressed && !prevDriftKey && isGrounded && speed > 2.0f) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) direction = DRIFT_LEFT;
        else if (IsKeyDown(KEY_RIGHT_SHIFT)) direction = DRIFT_RIGHT;
        
        if (direction != DRIFT_NONE) hopRequest = true;
    }

    if (!shiftPressed || speed < 1.0f) {
        direction = DRIFT_NONE;
        hopRequest = false;
    }

    float targetTilt = 0.0f;
    if (direction == DRIFT_LEFT) targetTilt = -7.0f;
    else if (direction == DRIFT_RIGHT) targetTilt = 7.0f;
    
    visualTilt = visualTilt + (targetTilt - visualTilt) * 8.0f * dt;

    prevDriftKey = shiftPressed;
}

float DriftController::GetGripModifier() const {
    return (direction != DRIFT_NONE) ? 1.8f : 9.0f;
}

float DriftController::GetTurnModifier() const {
    return (direction != DRIFT_NONE) ? 1.35f : 1.0f;
}