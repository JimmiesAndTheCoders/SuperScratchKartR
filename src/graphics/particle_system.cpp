#include "particle_system.h"
#include "raymath.h"

ParticleSystem::ParticleSystem() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].active = false;
    }
}

void ParticleSystem::EmitDust(Vector3 position, bool isGrass) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) {
            particles[i].active = true;
            particles[i].position = position;
            
            particles[i].velocity = {
                (float)GetRandomValue(-12, 12) / 10.0f,
                (float)GetRandomValue(4, 12) / 10.0f,
                (float)GetRandomValue(-12, 12) / 10.0f
            };
            
            particles[i].lifeSpan = (float)GetRandomValue(3, 7) / 10.0f;
            particles[i].maxLife = particles[i].lifeSpan;
            particles[i].size = (float)GetRandomValue(2, 4) / 10.0f;
            
            if (isGrass) {
                int colorType = GetRandomValue(0, 2);
                if (colorType == 0) particles[i].color = { 101, 67, 33, 255 }; 
                else if (colorType == 1) particles[i].color = { 139, 69, 19, 255 };
                else particles[i].color = { 160, 82, 45, 255 };
            } else {
                unsigned char gray = (unsigned char)GetRandomValue(180, 220);
                particles[i].color = { gray, gray, gray, 255 };
            }
            
            break; 
        }
    }
}

void ParticleSystem::Update(float dt) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
            particles[i].lifeSpan -= dt;
            if (particles[i].lifeSpan <= 0) {
                particles[i].active = false;
                continue;
            }

            particles[i].position = Vector3Add(particles[i].position, Vector3Scale(particles[i].velocity, dt));
            particles[i].size += dt * 0.5f;

            float alpha = (particles[i].lifeSpan / particles[i].maxLife);
            particles[i].color.a = (unsigned char)(alpha * 160);
        }
    }
}

void ParticleSystem::Draw() const {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
            DrawSphere(particles[i].position, particles[i].size * 0.5f, particles[i].color);
        }
    }
}