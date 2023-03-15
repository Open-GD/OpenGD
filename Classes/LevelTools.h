#pragma once

#include <string>

namespace LevelTools {
	bool verifyLevelIntegrity(std::string levelData, int id);
	
	std::string getAudioFilename(int lid);
	std::string getAudioTitle(int lid);
	std::string getArtistForAudio(int lid);
	std::string getURLForAudio(int lid);

	std::string base64EncodeString(std::string str);
	std::string base64DecodeString(std::string str);
}