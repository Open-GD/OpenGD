#pragma once

#include <string>
#include "SongObject.h"

namespace LevelTools {
	bool verifyLevelIntegrity(std::string levelData, int id);
	
	std::string getAudioFilename(int lid);
	std::string getAudioTitle(int lid);
	int getArtistForAudio(int lid);
	std::string getNameForArtist(int artistId);
	std::string getFbURLForArtist(int artistId);
	std::string getNgURLForArtist(int artistId);
	std::string getYtURLForArtist(int artistId);
	std::string getURLForAudio(int lid);
}
