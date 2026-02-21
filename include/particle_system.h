#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "raylib.h"
#include <vector>

struct Particle {
    Vector3 position;
    Vector3 velocity;
    float life;
    float maxLife;
    float size;
    float sizeGrowth;
    Color color;
};

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    void EmitDust(Vector3 position);
    void Update(float dt);
    void Draw() const;

private:
    std::vector<Particle> particles;
};

#endif