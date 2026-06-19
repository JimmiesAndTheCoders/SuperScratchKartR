#ifndef WHEEL_H
#define WHEEL_H

#include "raylib.h"

class Wheel {
public:
    Wheel();
    Wheel(Vector3 relativePos, float radius, bool canSteer);
    
    // Updated Draw to handle steering angle and rolling
    void Draw(Vector3 kartPos, float kartRotation, float steeringAngle, float rollAngle) const;

private:
    Vector3 offset; 
    float radius;
    bool isSteerable; // Only front wheels should turn
};

#endif