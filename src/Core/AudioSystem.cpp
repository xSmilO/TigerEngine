#include "Core/AudioSystem.h"
#include <iostream>

// This macro tells miniaudio to actually write the implementation code here.
// ONLY DO THIS IN ONE .cpp FILE!
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

namespace TigerEngine {

void *AudioSystem::s_Engine = nullptr;
void *AudioSystem::s_Music = nullptr;

void AudioSystem::Init() {
    ma_engine *engine = new ma_engine();

    ma_result result = ma_engine_init(NULL, engine);
    if(result != MA_SUCCESS) {
        std::cout << "Failed to initialize Audio Engine!" << std::endl;
        delete engine;
        return;
    }

    s_Engine = engine;
    std::cout << "Audio System Initialized." << std::endl;
}

void AudioSystem::PlayMusic(const std::string &filepath, bool loop) {
    if(!s_Engine)
        return;

    // If music is already playing, stop and destroy the old one first
    if(s_Music) {
        ma_sound_uninit((ma_sound *)s_Music);
        delete(ma_sound *)s_Music;
        s_Music = nullptr;
    }

    // Create a new sound object
    s_Music = new ma_sound();

    // Initialize the sound from the file
    ma_result result =
        ma_sound_init_from_file((ma_engine *)s_Engine, filepath.c_str(), 0, NULL, NULL, (ma_sound *)s_Music);

    if(result == MA_SUCCESS) {
        // Tell miniaudio to loop it, then start playing!
        ma_sound_set_looping((ma_sound *)s_Music, loop ? MA_TRUE : MA_FALSE);
        ma_sound_start((ma_sound *)s_Music);
    } else {
        std::cout << "Failed to load music: " << filepath << std::endl;
        delete(ma_sound *)s_Music;
        s_Music = nullptr;
    }
}

void AudioSystem::StopMusic() {
    if(s_Music) {
        ma_sound_stop((ma_sound *)s_Music);
    }
}

void AudioSystem::Shutdown() {
    if(s_Music) {
        ma_sound_uninit((ma_sound *)s_Music);
        delete(ma_sound *)s_Music;
        s_Music = nullptr;
    }

    if(s_Engine) {
        ma_engine_uninit((ma_engine *)s_Engine);
        delete(ma_engine *)s_Engine;
        s_Engine = nullptr;
    }
}

void AudioSystem::PlaySound2D(const std::string &filepath) {
    if(!s_Engine)
        return;

    // ma_engine_play_sound handles loading and playing the file automatically
    ma_engine_play_sound((ma_engine *)s_Engine, filepath.c_str(), NULL);
}

void AudioSystem::SetGlobalVolume(float volume) {
    if(s_Engine) {
        // This affects all sounds played via PlaySound2D
        ma_engine_set_volume((ma_engine *)s_Engine, volume);
    }
}

void AudioSystem::SetMusicVolume(float volume) {
    if(s_Music) {
        // This affects only the currently playing background music
        ma_sound_set_volume((ma_sound *)s_Music, volume);
    }
}

} // namespace TigerEngine