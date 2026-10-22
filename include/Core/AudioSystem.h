#pragma once
#include <string>

namespace TigerEngine {

class AudioSystem {
  public:
    static void Init();
    static void Shutdown();

    // Plays a sound immediately (fire and forget)
    static void PlaySound2D(const std::string &filepath);

    static void PlayMusic(const std::string &filepath, bool loop = true);
    static void StopMusic();

    static void SetGlobalVolume(float volume);
    static void SetMusicVolume(float volume);

  private:
    // We hide the miniaudio engine pointer as a void* in the header
    // so we don't have to include miniaudio.h everywhere in the engine.
    static void *s_Engine;
    static void *s_Music;
};

} // namespace TigerEngine