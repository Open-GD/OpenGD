#include "LevelTools.h"
#include "base64.h"
#include "constants.h"
#include <cstring>

bool LevelTools::verifyLevelIntegrity(std::string levelData, int id) {

	if(!levelData.starts_with("H4sIAAAAAAAA") && id <= 10) return false || FORCE_LOAD_LEVEL;
	return true;
}

std::string LevelTools::getAudioFilename(int lid) {
	switch(lid) {
		case 1:  return "BackOnTrack.mp3";
		case 2:  return "Polargeist.mp3";
		case 3:  return "DryOut.mp3";
		case 4:  return "BaseAfterBase.mp3";
		case 5:  return "CantLetGo.mp3";
		case 6:  return "Jumper.mp3";
		case 7:  return "TimeMachine.mp3";
		case 8:  return "Cycles.mp3";
		case 9:  return "xStep.mp3";
		case 10:  return "Clutterfunk.mp3";
		case 11:  return "TheoryOfEverything.mp3";
		case 12:  return "Electroman.mp3";
		case 13:  return "Clubstep.mp3";
		case 14:  return "Electrodynamix.mp3";
		case 15:  return "HexagonForce.mp3";
		case 16:  return "BlastProcessing.mp3";
		case 17:  return "TheoryOfEverything2.mp3";
		case 18:  return "GeometricalDominator.mp3";
		case 19:  return "Deadlocked.mp3";
		case 20:  return "Fingerdash.mp3";
		default: return "StereoMadness.mp3";
	}
}

std::string LevelTools::getAudioTitle(int lid) {
	switch(lid) {
		case 1:  return "Back On Track";
		case 2:  return "Polargeist";
		case 3:  return "Dry Out";
		case 4:  return "Base After Base";
		case 5:  return "Cant Let Go";
		case 6:  return "Jumper";
		case 7:  return "Time Machine";
		case 8:  return "Cycles";
		case 9:  return "xStep";
		case 10:  return "Clutterfunk.";
		case 11:  return "Theory Of Everything";
		case 12:  return "Electroman";
		case 13:  return "Clubstep";
		case 14:  return "Electrodynamix";
		case 15:  return "Hexagon Force";
		case 16:  return "Blast Processing";
		case 17:  return "Theory Of Everything 2";
		case 18:  return "Geometrical Dominator";
		case 19:  return "Deadlocked";
		case 20:  return "Fingerdash";
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
