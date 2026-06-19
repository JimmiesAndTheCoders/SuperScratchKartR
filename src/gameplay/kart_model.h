#ifndef KART_MODEL_H
#define KART_MODEL_H

#include "raylib.h"

enum class KartAnimState {
    IDLE = 0,
    STEER_LEFT,
    STEER_RIGHT,
    ACCELERATE
};

class KartModel {
public:
    KartModel(const char* modelPath);
    ~KartModel();

    void Update(float dt);
    
    void Draw(Vector3 position, float rotationY, float tiltZ, float squish, 
              float wheelSpin, float steerAngle, Color tint) const;

    void SetAnimationState(KartAnimState state);
    void ApplyTexture(const char* texturePath);
    bool HasValidModel() const { return hasValidModel; }

private:
    Model model;
    ModelAnimation* animations;
    int animCount;
    
    int currentAnimIndex;
    int animFrameCounter;

    bool hasValidModel;
    
    Transform* originalBindPose; 
};

#endif