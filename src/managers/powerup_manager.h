#ifndef POWERUP_MANAGER_H
#define POWERUP_MANAGER_H

#include "raylib.h"
#include "track.h"
#include "kart.h"
#include <vector>

// type alias: use the same enum that kart uses for slots
using PowerUpSlot = ::PowerUpSlot;

struct CapsuleInstance {
    Vector3 position;
    float rotation;
    float scale;
    PowerUpSlot type;
    bool active;
};

class PowerUpManager {
public:
    PowerUpManager();
    ~PowerUpManager();

    // loads the capsule model from disk (hardcoded path)
    void LoadModel();
    // randomly scatter capsules on the road surface of a track
    void GenerateCapsules(const Track* track, int count, float areaSize);

    // update logic (pickup detection)
    void Update(Kart* player);
    void Draw() const;

private:
    Model capsuleModel;
    std::vector<CapsuleInstance> capsules;

    PowerUpSlot RandomPowerUp() const;
};

#endif
