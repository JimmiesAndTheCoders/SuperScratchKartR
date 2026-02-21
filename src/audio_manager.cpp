#include "audio_manager.h"
#include <math.h>

AudioManager::AudioManager() : audioReady(false) {
    InitAudioDevice();

    if (IsAudioDeviceReady()) {
        audioReady = true;

        raceTheme = LoadMusicStream("assets/audio/music/race_theme.mp3");
        engineLoop = LoadSound("assets/audio/sounds/engine_loop.wav");
        driftSound = LoadSound("assets/audio/sounds/drift.wav");

        raceTheme.looping = true;
        PlaySound(engineLoop);
    }
}

AudioManager::~AudioManager() {
    if (audioReady) {
        UnloadMusicStream(raceTheme);
        UnloadSound(engineLoop);
        UnloadSound(driftSound);
        CloseAudioDevice();
    }
}

void AudioManager::Update(float kartSpeed, float maxSpeed) {
    if (!audioReady) return;

    UpdateMusicStream(raceTheme);

    float speedRatio = fabsf(kartSpeed) / maxSpeed;
    float targetPitch = 0.8f + (speedRatio * 1.2f);
    SetSoundPitch(engineLoop, targetPitch);

    if (!IsSoundPlaying(engineLoop)) {
        PlaySound(engineLoop);
    }
}

void AudioManager::UpdateDriftSound(bool isDrifting) {
    if (!audioReady) return;

    if (isDrifting) {
        if (!IsSoundPlaying(driftSound)) {
            PlaySound(driftSound);
        }
    } else {
        if (IsSoundPlaying(driftSound)) {
            StopSound(driftSound);
        }
    }
}

void AudioManager::PlayMusic() {
    if (audioReady) PlayMusicStream(raceTheme);
}

void AudioManager::StopMusic() {
    if (audioReady) StopMusicStream(raceTheme);
}