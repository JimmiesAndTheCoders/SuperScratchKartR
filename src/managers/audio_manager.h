#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "raylib.h"

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void Update(float kartSpeed, float maxSpeed);
    void UpdateDriftSound(bool isDrifting); 
    
    void PlayMusic();
    void StopMusic();

private:
    Music raceTheme;
    Sound engineLoop;
    Sound driftSound;

    bool audioReady;
};

#endif