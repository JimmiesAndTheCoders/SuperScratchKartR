#ifndef DECORATION_MANAGER_H
#define DECORATION_MANAGER_H

#include "raylib.h"
#include "track.h"
#include <vector>

struct DecorationInstance {
    Vector3 position;
    float scale;
    float rotation;
    int modelIndex;
};

class DecorationManager {
public:
    DecorationManager();
    ~DecorationManager();
    void LoadTreeModels();
    void GenerateScatteredTrees(const Track* track, int count, float areaSize);
    void Draw() const;

private:
    std::vector<Model> treeModels;
    std::vector<DecorationInstance> instances;
    bool IsValidPlacement(Vector3 pos, const Track* track) const;
};

#endif