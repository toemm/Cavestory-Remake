#pragma once

#include <SDL_mixer.h>
#include "globals.h"

#include <string>
#include <map>

class Audio
{

public:
	Audio();
	~Audio();

	void initSounds();
	void initMusic();

	static void playSound(std::string name, int nr);
	static void stopSound(std::string name);

	static void playMusic(std::string name, int nr);
	static void pauseMusic();
	static void stopMusic(std::string name);

	static bool isPaused;


private:
	static std::map<std::string, Mix_Chunk *> _soundArchive;
	static std::map<std::string, Mix_Music *> _musicArchive;

};

