#include "audio_manager.h"
#include <math.h>

AudioManager::AudioManager() : audioReady(false) {
    InitAudioDevice();

    if (IsAudioDeviceReady()) {
        audioReady = true;

        raceTheme = LoadMusicStream("assets/audio/music/race_theme.mp3");
        engineLoop = LoadSound("assets/audio/sounds/engine_loop.wav");
        driftLoop = LoadSound("assets/audio/sounds/drift.wav");

        raceTheme.looping = true;
        
        PlaySound(engineLoop);
    }
}

AudioManager::~AudioManager() {
    if (audioReady) {
        UnloadMusicStream(raceTheme);
        UnloadSound(engineLoop);
        UnloadSound(driftLoop);
        CloseAudioDevice();
    }
}

void AudioManager::Update(float kartSpeed, float maxSpeed, bool isDrifting) {
    if (!audioReady) return;

    UpdateMusicStream(raceTheme);

    // Engine Pitch
    float speedRatio = fabsf(kartSpeed) / maxSpeed;
    float targetPitch = 0.8f + (speedRatio * 1.2f);
    SetSoundPitch(engineLoop, targetPitch);

    if (!IsSoundPlaying(engineLoop)) PlaySound(engineLoop);

    // Drift Sound Logic
    if (isDrifting) {
        if (!IsSoundPlaying(driftLoop)) PlaySound(driftLoop);
        // Fade in or maintain volume
        SetSoundVolume(driftLoop, 0.7f);
    } else {
        // Quickly fade out or stop
        StopSound(driftLoop);
    }
}

void AudioManager::PlayMusic() {
    if (audioReady) PlayMusicStream(raceTheme);
}

void AudioManager::StopMusic() {
    if (audioReady) StopMusicStream(raceTheme);
}