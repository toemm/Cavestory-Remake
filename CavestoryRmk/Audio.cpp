#include "Audio.h"

std::map<std::string, Mix_Chunk *> Audio::_soundArchive;
std::map<std::string, Mix_Music *> Audio::_musicArchive;
bool Audio::isPaused = false;

Audio::Audio()
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	this->initSounds();
	this->initMusic();

	// set music to proper volume 
	Mix_VolumeMusic(audio::MUSIC_VOLUME);

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
	Mix_Chunk * pistolShot = Mix_LoadWAV("content/sounds/pistol.wav");
	Mix_VolumeChunk(pistolShot, audio::WEAPON_VOLUME);								// chunkies have to be volume initted this way, maybe better way?
	this->_soundArchive.insert({ "pistol", pistolShot });

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
	Mix_PlayChannel(-1, Audio::_soundArchive.at(name), nr);
}

void Audio::stopSound(std::string name)
{

}

void Audio::playMusic(std::string name, int nr)
{
	// If music is not playing, start it
	if (Mix_PlayingMusic() == 0) {
		Audio::isPaused = false;
		Mix_PlayMusic(Audio::_musicArchive.at(name), nr);
	}
	// The music is paused, resume it
	else {
		Audio::isPaused = false;
		Mix_ResumeMusic();
	}
}

void Audio::pauseMusic()
{
	if (Mix_PlayingMusic() == 1) {
		Audio::isPaused = true;
		Mix_PauseMusic();
	}
}

void Audio::stopMusic(std::string name)
{
	Mix_HaltMusic();
}