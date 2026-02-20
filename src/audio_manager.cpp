#include "audio_manager.h"
#include <math.h>

AudioManager::AudioManager() : audioReady(false) {
    InitAudioDevice();
    
    if (IsAudioDeviceReady()) {
        audioReady = true;
        
        // Load Music and Sounds
        raceTheme = LoadMusicStream("assets/music/race_theme.mp3");
        engineLoop = LoadSound("assets/sounds/engine_loop.wav");
        
        raceTheme.looping = true;
        
        // Start the engine loop immediately
        PlaySound(engineLoop);
    }
}

AudioManager::~AudioManager() {
    if (audioReady) {
        UnloadMusicStream(raceTheme);
        UnloadSound(engineLoop);
        CloseAudioDevice();
    }
}

void AudioManager::Update(float kartSpeed, float maxSpeed) {
    if (!audioReady) return;

    // Update Music Stream Buffer
    UpdateMusicStream(raceTheme);

    // Calculate Pitch based on speed
    // Base pitch is 0.8f, max pitch is 2.0f
    float speedRatio = fabsf(kartSpeed) / maxSpeed;
    float targetPitch = 0.8f + (speedRatio * 1.2f);
    
    SetSoundPitch(engineLoop, targetPitch);

    // Ensure engine loop keeps playing if it ends
    if (!IsSoundPlaying(engineLoop)) {
        PlaySound(engineLoop);
    }
}

void AudioManager::PlayMusic() {
    if (audioReady) PlayMusicStream(raceTheme);
}

void AudioManager::StopMusic() {
    if (audioReady) StopMusicStream(raceTheme);
}
