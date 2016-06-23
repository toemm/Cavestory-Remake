#include "Audio.h"


Audio::Audio()
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
}


Audio::~Audio()
{

}

void Audio::initSounds()
{
	// Initialize sound effects

	// Weapon sounds
	this->_soundArchive.insert({ "pistol", Mix_LoadWAV("content/sounds/pistol.wav") });

	// Item Pickup

	// Healthpickup

	// LevelUp



}

void Audio::initMusic()
{

}

void Audio::playSound(std::string name, int nr)
{

}

void Audio::stopSound(std::string name)
{

}

void Audio::playMusic(std::string name, int nr)
{

}

void Audio::stopMusic(std::string name)
{

}