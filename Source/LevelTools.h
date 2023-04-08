#pragma once

#include <string>
#include "SongObject.h"

#define ARTIST_DJVI 0
#define ARTIST_WATERFLAME 1
#define ARTIST_OCULAR 2
#define ARTIST_FBOUND 3
#define ARTIST_STEP 4
#define ARTIST_DJNATE 5
#define ARTIST_F777 6
#define ARTIST_MDK 7

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
