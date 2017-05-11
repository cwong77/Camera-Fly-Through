#include "Windows.h"
#include "Mmsystem.h"
#include "Winnt.h"
//#include <dirent.h>
#include <filesystem>
#include <string>
//#include <FileAPI.h>
#include <cmath>
#include <iostream>

#include "include/irrKlang.h"	//sound player

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")


class Jukebox {

	irrklang::ISoundEngine* se;
	irrklang::ISound* currentSound;
	irrklang::ik_f32 volume;

	std::vector<std::string> playlist;	//holds playlist
	int playlistIndex;	//where we are in the playlist
	bool playing = false; //are we playing sounds right now?

	void load();	//load playlist from music folder
	void play(std::string filepath);	//play sound, handles whether or not the jukebox is playing. to be used with background sounds

public:
	Jukebox();
	void init();	//init it with menu music
	void playSFX(std::string _path);	//does no check if already playing
	void playRandom();	//plays a random song from playlist
	void next();	//play next song in playlist - will end any other sounds 
	bool isPlaying();	//is the jukebox playing right now?
	void stop();	//stop everything
	void raiseVolume();

	irrklang::ISoundEngine* SoundEngine();	//access to sound engine

	~Jukebox();
};