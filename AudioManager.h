#pragma once
#include <SFML/Audio.hpp>
#include <map>

class AudioManager
{

public:
    AudioManager();
    ~AudioManager();

    void LoadSound(const std::string& name, const std::string& filename);
    void PlaySound(const std::string& name, float volume = 100.f);
    void StopSound(const std::string& name);
    void PlayMusic(const std::string& name, float volume = 50.f, bool loop = true);
    void StopMusic();
    void SetSoundVolume(float volume);
    void SetMusicVolume(float volume);

private:
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;
    sf::Music backgroundMusic;
    float soundVolume;
    float musicVolume;
};