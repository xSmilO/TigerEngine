#pragma once
#include <string>

namespace TigerEngine {

class AudioSystem {
  public:
    static void Init();
    static void Shutdown();

    static void PlaySound2D(const std::string &filepath);

    static void PlayMusic(const std::string &filepath, bool loop = true);
    static void PauseMusic();
    static void ResumeMusic();
    static void RestartMusic();
    static void StopMusic();
    static bool IsMusicPlaying();

    static void SetGlobalVolume(float volume);
    static void SetMusicVolume(float volume);

  private:

    static void *s_Engine;
    static void *s_Music;
};

}
