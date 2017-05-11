#include "Jukebox.h"

Jukebox::Jukebox() {

}

void Jukebox::init() {
	se = irrklang::createIrrKlangDevice();
	if (!se)
		printf("Error: Sound Engine could not be created\n");
	//load();	//load playlist
	playing = true;
	//playlistIndex = 0;	//initiates playlist index
	//currentSound = se->play2D(_path.c_str());
	currentSound = se->play3D("../Sound/nature_sound.wav", irrklang::vec3df(-7, 0, 0), true, false, true);	//looped
	//se->play3D("../Sound/town_sound.mp3", irrklang::vec3df(2, 0, 0), true);	//looped

}

void Jukebox::load() {
	WIN32_FIND_DATA FindFileData;
	HANDLE hfind;
	char* path = "../Sound/*.wav";
	hfind = FindFirstFile(path, &FindFileData);
	if (hfind == INVALID_HANDLE_VALUE) {
		printf("FindFirstFailed\n");
	}
	else {
		do{
			std::string path = "../Sound/";	//attach directory path
			playlist.push_back(path + FindFileData.cFileName);
		} while (FindNextFile(hfind, &FindFileData) != 0);
	}
}

void Jukebox::playSFX(std::string _path) {
	se->play2D(_path.c_str());
}

void Jukebox::playRandom() {
	int rand = std::rand() % playlist.size();
	play(playlist[rand]);
}

void Jukebox::play(std::string _path) {
	if (!playing) {	//if not playing, play sound
		playing = true;
		se->play2D(_path.c_str(), false, false , true);
	}
	else {	//else if already playing, stop current sound, play new one
		se->stopAllSounds();
		currentSound = se->play2D(_path.c_str());
	}
}

void Jukebox::next() {
	if (++playlistIndex < playlist.size())	//check if still in playlist
		play(playlist[playlistIndex]);
	else {
		playlistIndex = 0;	//resets playlist
		play(playlist[playlistIndex]);
	}
}

void Jukebox::stop() {
	se->stopAllSounds();
}

bool Jukebox::isPlaying() {
	return playing;
}

void Jukebox::raiseVolume() {
	if (currentSound != NULL) {
		irrklang::ik_f32 vol = currentSound->getVolume();
		currentSound->setVolume(++vol);
	}
	//std::cout << "Current sound pos: ";
	//std::cout << currentSound->getPosition().X << " " << currentSound->getPosition().Y << " " << currentSound->getPosition().Z <<std::endl;
}

irrklang::ISoundEngine * Jukebox::SoundEngine()
{
	return se;
}

Jukebox::~Jukebox() {
	currentSound->drop();
	se->drop();
}