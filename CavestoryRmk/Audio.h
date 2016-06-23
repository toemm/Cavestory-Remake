#pragma once

#include <SDL_mixer.h>

#include <string>
#include <map>

class Audio
{

public:
	Audio();
	~Audio();

	void initSounds();
	void initMusic();

	void playSound(std::string name, int nr);
	void stopSound(std::string name);

	void playMusic(std::string name, int nr);
	void pauseMusic();
	void stopMusic(std::string name);

	bool isPaused;


private:
	std::map<std::string, Mix_Chunk *> _soundArchive;
	std::map<std::string, Mix_Music *> _musicArchive;

};

