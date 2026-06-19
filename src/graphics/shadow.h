#ifndef SHADOW_H
#define SHADOW_H

#include "raylib.h"

class Shadow {
public:
    Shadow();
    ~Shadow();
    void Draw(Vector3 position, float radius, float opacity) const;

private:
    Texture2D shadowTexture;
};

#endif