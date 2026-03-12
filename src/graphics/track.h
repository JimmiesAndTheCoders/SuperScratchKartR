#ifndef TRACK_H
#define TRACK_H

#include "raylib.h"

enum class SurfaceType {
    ROAD,
    GRASS,
    NONE
};

class Track {
public:
    Track(const char* modelPath);
    ~Track();

    void Draw() const;

    bool GetGroundInfo(Vector3 position, float& outHeight, SurfaceType& outType) const;

private:
    Model trackModel;
    float safetyFloorHeight = 0.0f;
};

#endif