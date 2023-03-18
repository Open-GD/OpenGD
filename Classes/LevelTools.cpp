#include "LevelTools.h"
#include "base64.h"

bool LevelTools::verifyLevelIntegrity(std::string levelData, int id) {
	if(!levelData.starts_with("H4sIAAAAAAAA") && id <= 10) return false;
	return true;
}

std::string LevelTools::getAudioFilename(int lid) {
	switch(lid) {
		case 0:  return "StereoMadness.mp3";
		case 1:  return "BackOnTrack.mp3";
		case 2:  return "Polargeist.mp3";
		case 3:  return "DryOut.mp3";
		case 4:  return "BaseAfterBase.mp3";
		case 5:  return "CantLetGo.mp3";
		case 6:  return "Jumper.mp3";
		default: return "StereoMadness.mp3";
	}
}

std::string LevelTools::getAudioTitle(int lid) {
	switch(lid) {
		case 0:  return "Stereo Madness";
		case 1:  return "Back On Track";
		case 2:  return "Polargeist";
		case 3:  return "Dry Out";
		case 4:  return "Base After Base";
		case 5:  return "Cant Let Go";
		case 6:  return "Jumper";
		default: return "Stereo Madness";
	}
}
std::string LevelTools::getArtistForAudio(int lid) {
	switch(lid) {
		case 0:  return "ForeverBound";
		case 1:  return "DJVI";
		case 2:  return "Step";
		case 3:  return "DJVI";
		case 4:  return "DJVI";
		case 5:  return "DJVI";
		case 6:  return "Waterflame";
		default: return "ForeverBound";
	}
}
std::string LevelTools::getURLForAudio(int lid) {
	switch(lid) {
		case 0:  return "https://www.youtube.com/watch?v=JhKyKEDxo8Q";
		case 1:  return "https://www.youtube.com/watch?v=N9vDTYZpqXM";
		case 2:  return "https://www.youtube.com/watch?v=4W28wWWxKuQ";
		case 3:  return "https://www.youtube.com/watch?v=FnXabH2q2A0";
		case 4:  return "https://www.youtube.com/watch?v=TZULkgQPHt0";
		case 5:  return "https://www.youtube.com/watch?v=fLnF-QnR1Zw";
		case 6:  return "https://www.youtube.com/watch?v=ZXHO4AN_49Q";
		default: return "https://www.youtube.com/watch?v=JhKyKEDxo8Q";
	}
}

std::string LevelTools::base64EncodeString(std::string str) {
	char *buffer = (char *)malloc(str.size());
	memcpy(buffer, str.data(), str.size());

	return base64_encode((const BYTE *)buffer, str.size());
}
std::string LevelTools::base64DecodeString(std::string str) {
	return std::string((char *)base64_decode(str).data());
}