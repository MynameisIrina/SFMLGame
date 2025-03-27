#include "managers/AudioManager.h"
#include <iostream>


AudioManager::AudioManager()
    : soundVolume(100.f), musicVolume(50.f) {}


AudioManager::~AudioManager() {
    StopMusic();
}

void AudioManager::LoadSound(const std::string &name, const std::string &filename)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename))
    {
        std::cout << "Error loading sound: " << filename << std::endl;
        return;
    }
    soundBuffers[name] = std::move(buffer);
}

void AudioManager::PlaySound(const std::string &name, float volume)
{
    if(soundBuffers.find(name) == soundBuffers.end())
    {
        std::cerr << "Sound not loaded: " << name << std::endl;
        return;
    }

    sf::Sound sound;
    sound.setBuffer(soundBuffers[name]);
    sound.setVolume(volume);
    sounds[name] = std::move(sound);
    sounds[name].play();
}

void AudioManager::StopSound(const std::string &name)
{
    if(sounds.find(name) != sounds.end())
    {
        sounds[name].stop();
    }

}

void AudioManager::PlayMusic(const std::string &filename, float volume, bool loop)
{
    if (!backgroundMusic.openFromFile(filename))
    {
        std::cerr << "Error loading music: " << filename << std::endl;
        return;
    }
    
    backgroundMusic.setVolume(volume);
    backgroundMusic.setLoop(loop);
    backgroundMusic.play();
}

void AudioManager::StopMusic()
{
    backgroundMusic.stop();
}

void AudioManager::SetSoundVolume(float volume)
{
    soundVolume = volume;
}

void AudioManager::SetMusicVolume(float volume)
{
    musicVolume = volume;
}
