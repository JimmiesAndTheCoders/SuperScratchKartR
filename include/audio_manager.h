#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "raylib.h"

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void Update(float kartSpeed, float maxSpeed, bool isDrifting);
    void PlayMusic();
    void StopMusic();

private:
    Music raceTheme;
    Sound engineLoop;
    Sound driftLoop;

    bool audioReady;
};

#endif