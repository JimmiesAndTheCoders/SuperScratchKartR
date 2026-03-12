#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "raylib.h"

#define MAX_PARTICLES 250

struct Particle {
    Vector3 position;
    Vector3 velocity;
    Color color;
    float lifeSpan;
    float maxLife;
    float size;
    bool active;
};

class ParticleSystem {
public:
    ParticleSystem();
    void Update(float dt);
    void Draw() const;
    void EmitDust(Vector3 position, bool isGrass);

private:
    Particle particles[MAX_PARTICLES];
};

#endif