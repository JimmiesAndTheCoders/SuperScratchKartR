#include "wheel.h"
#include "raymath.h"
#include <math.h>

Wheel::Wheel() : offset({0,0,0}), radius(0.4f), isSteerable(false) {}

Wheel::Wheel(Vector3 relativePos, float r, bool canSteer) {
    offset = relativePos;
    radius = r;
    isSteerable = canSteer;
}

void Wheel::Draw(Vector3 kartPos, float kartRotation, float steeringAngle, float rollAngle) const {
    float rad = kartRotation * DEG2RAD;
    
    // Position relative to body
    float rotatedX = offset.x * cosf(rad) + offset.z * sinf(rad);
    float rotatedZ = -offset.x * sinf(rad) + offset.z * cosf(rad);
    Vector3 worldPos = { kartPos.x + rotatedX, kartPos.y + offset.y, kartPos.z + rotatedZ };

    // Total rotation for the wheel direction
    float totalRotation = kartRotation;
    if (isSteerable) totalRotation += steeringAngle;

    // Tire dimensions
    float wheelWidth = 0.5f;
    float dirX = cosf(totalRotation * DEG2RAD);
    float dirZ = -sinf(totalRotation * DEG2RAD);

    // Tire points (The Black Rubber)
    Vector3 startPos = { worldPos.x - dirX * (wheelWidth/2), worldPos.y, worldPos.z - dirZ * (wheelWidth/2) };
    Vector3 endPos   = { worldPos.x + dirX * (wheelWidth/2), worldPos.y, worldPos.z + dirZ * (wheelWidth/2) };

    // Rim points (The Gray Metal) 
    // We make this slightly shorter to avoid Z-Fighting with the tire faces
    float innerRimWidth = wheelWidth * 0.98f;
    
    // Let's use the SHORTER method for the side walls:
    Vector3 rimStart = { worldPos.x - dirX * (innerRimWidth/2), worldPos.y, worldPos.z - dirZ * (innerRimWidth/2) };
    Vector3 rimEnd   = { worldPos.x + dirX * (innerRimWidth/2), worldPos.y, worldPos.z + dirZ * (innerRimWidth/2) };

    // 1. Draw the Black Tire (Main Body)
    DrawCylinderEx(startPos, endPos, radius, radius, 16, BLACK);
    
    // 2. Draw the Gray Rim (Inner Hub)
    // By using innerRimWidth, the flat faces of the gray cylinder are slightly 
    // inside the black one, stopping the flickering.
    DrawCylinderEx(rimStart, rimEnd, radius * 0.7f, radius * 0.7f, 16, DARKGRAY);
    
    // 3. Optional: Add a small hubcap "dot" on the very outside to show spin
    // This is offset by just 0.01f to ensure it stays on top
    Vector3 hubCapPos = { worldPos.x + dirX * (wheelWidth/2 + 0.01f), worldPos.y, worldPos.z + dirZ * (wheelWidth/2 + 0.01f) };
    
    // Fixed DrawCircle3D parameters: (center, radius, rotationAxis, rotationAngle, color)
    DrawCircle3D(hubCapPos, radius * 0.3f, (Vector3){ 0, 1, 0 }, totalRotation + 90.0f, RED);
}
