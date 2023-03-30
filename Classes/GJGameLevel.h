#pragma once

#include <string>
#include <vector>
#include <axmol.h>
#include "GameToolbox.h"

#define PKSTRING 0
#define PKINT 1
#define PKBOOL 2
#define PKBASE64 3

enum DifficultyType {
	kMainLevels = 0,
	kLevelCell,
	kLevelInfoLayer
};

class GJGameLevel { 
public:

	//initialize everything otherwise some stuff just gets set to strange values idk

	int _LevelID = 0;
	int _Version = 0;
	int _PlayerID = 0;
	int _Downloads = 0;
	int _MusicID = 0;
	int _Likes = 0;
	int _Length = 0;
	int _DifficultyDenominator = 0;
	int _DifficultyNumerator = 0;
	int _SetCompletes = 0;
	int _GameVersion = 0;
	int _Dislikes = 0;
	int _Stars = 0;
	int _FeatureScore = 0;
	int _CopiedID = 0;
	int _Coins = 0;
	int _StarsRequested = 0;
	int _DailyNumber = 0;
	int _Epic = 0;
	int _DemonDifficulty = 0;
	int _Objects = 0;
	int _EditorTime = 0;
	int _EditorTimeTotal = 0;
	int _SongID = 0;
	int _OfficialSongID = 0;

	float _normalPercent = 0.f;
	float _practicePercent = 0.f;
	
	bool _Auto = false;
	bool _Demon = false;
	bool _VerifiedCoins = false;
	bool _LDM = false;
	bool _Gauntlet = false;
	bool _2P = false;

	std::string _LevelName{};
	std::string _Description{};
	std::string _LevelString{};
	std::string _XORPassword{};
	std::string _UploadDate{};
	std::string _UpdateDate{};
	std::string _ExtraString{};
	std::string _Settings{};
	std::string _RecordString{};
	std::string _SondURL{};
	std::string _SongName{};
	std::string _LevelCreator{};

	// Expects RobTop like string. If it not, nullptr gets returned.
	static GJGameLevel *createWithResponse(std::string backendResponse);
	GJGameLevel(){}
	GJGameLevel(std::string levelName, int levelID);
	static GJGameLevel *createWithMinimumData(
		std::string levelName, 
		std::string creatorNickname, 
		int levelID
	);
	static GJGameLevel *create();

	static std::string getLevelStrFromID(int gdLevelID);
	static std::string decompressLvlStr(std::string compressedLvlStr);

	static std::string getDifficultySprite(GJGameLevel* level, DifficultyType type = kLevelCell);
};
