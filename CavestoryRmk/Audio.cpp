#include "Audio.h"


Audio::Audio() :
isPaused(false)
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	this->initSounds();
	this->initMusic();

}


Audio::~Audio()
{
	// Free sounds
	for (const auto& soundpair : this->_soundArchive) {
		Mix_FreeChunk(soundpair.second);
	}

	// Free music
	for (const auto& soundpair : this->_musicArchive) {
		Mix_FreeMusic(soundpair.second);
	}

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
	// Themes
	this->_musicArchive.insert({ "maintheme", Mix_LoadMUS("content/sounds/maintheme.wav") });



}

void Audio::playSound(std::string name, int nr)
{
	Mix_PlayChannel(-1, this->_soundArchive.at(name), nr);
}

void Audio::stopSound(std::string name)
{

}

void Audio::playMusic(std::string name, int nr)
{
	// If music is not playing, start it
	if (Mix_PlayingMusic() == 0) {
		this->isPaused = false;
		Mix_PlayMusic(this->_musicArchive.at(name), nr);
	}
	// The music is paused, resume it
	else {
		this->isPaused = false;
		Mix_ResumeMusic();
	}
}

void Audio::pauseMusic()
{
	if (Mix_PlayingMusic() == 1) {
		this->isPaused = true;
		Mix_PauseMusic();
	}
}

void Audio::stopMusic(std::string name)
{
	Mix_HaltMusic();
}