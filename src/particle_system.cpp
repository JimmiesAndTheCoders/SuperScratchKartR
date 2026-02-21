#include "particle_system.h"
#include "raymath.h"

ParticleSystem::ParticleSystem() {}

ParticleSystem::~ParticleSystem() {
    particles.clear();
}

void ParticleSystem::EmitDust(Vector3 position) {
    Particle p;
    p.position = position;
    
    // Slight random outward pop for the dust
    p.velocity = { 
        (float)GetRandomValue(-15, 15) / 10.0f, 
        (float)GetRandomValue(5, 20) / 10.0f, 
        (float)GetRandomValue(-15, 15) / 10.0f 
    };
    
    p.life = 0.6f;
    p.maxLife = 0.6f;
    p.size = 0.1f;         // Start small
    p.sizeGrowth = 2.0f;   // Expand rapidly like a cartoon cloud
    p.color = { 220, 220, 220, 255 }; // Light gray/white dust
    
    particles.push_back(p);
}

void ParticleSystem::Update(float dt) {
    for (size_t i = 0; i < particles.size(); ) {
        particles[i].life -= dt;
        particles[i].position = Vector3Add(particles[i].position, Vector3Scale(particles[i].velocity, dt));
        particles[i].size += particles[i].sizeGrowth * dt;
        
        if (particles[i].life <= 0.0f) {
            particles.erase(particles.begin() + i);
        } else {
            i++;
        }
    }
}

void ParticleSystem::Draw() const {
    for (size_t i = 0; i < particles.size(); i++) {
        const Particle& p = particles[i];
        float alpha = p.life / p.maxLife; // Fade out over time
        
        Color renderColor = p.color;
        renderColor.a = (unsigned char)(255.0f * alpha);
        
        // Drawing simple spheres works perfectly for cartoon clouds without needing camera billboarding
        DrawSphere(p.position, p.size, renderColor);
    }
}