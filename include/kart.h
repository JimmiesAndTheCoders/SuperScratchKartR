#ifndef KART_H
#define KART_H

#include "raylib.h"
#include "kart_model.h"
#include "particle_system.h"
#include "shadow.h"
#include "suspension_system.h"

class PhysicsController;
class Track;

class Kart {
public:
    Kart(const char* modelPath);
    Kart(Vector3 startPos, const char* modelPath);
    ~Kart();

    void Update(const Track* currentTrack);
    void Draw() const;
    
    Vector3 GetPosition() const { return position; }
    float GetRotation() const { return rotation; }
    bool IsDrifting() const { return isDrifting; }
    float GetSpeed() const;

private:
    Vector3 position;
    Vector3 velocity;
    float rotation; 
    float velocityY;
    float currentGroundHeight;
    bool isDrifting;
    
    float wheelSpin;
    struct {
        float tilt;
        float squish;
        Vector3 visualOffset;
    } juice;

    KartModel visualModel;
    ParticleSystem dustParticles;
    Shadow blobShadow;
    PhysicsController* physics;
    SuspensionSystem suspension;
};

#endif