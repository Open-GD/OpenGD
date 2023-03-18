#pragma once

#include <string>
#include <vector>
#include <axmol.h>
#include <zlib.h>

#define PKSTRING 0
#define PKINT 1
#define PKBOOL 2
#define PKBASE64 3

class GJGameLevel { 
public:
	int _LevelID;
	int _Version;
	int _PlayerID;
	int _Downloads;
	int _MusicID;
	int _Likes;
	int _Length;
	int _DifficultyDenominator;
	int _DifficultyNumerator;
	int _SetCompletes;
	int _GameVersion;
	int _Dislikes;
	int _Stars;
	int _FeatureScore;
	int _CopiedID;
	int _Coins;
	int _StarsRequested;
	int _DailyNumber;
	int _Epic;
	int _DemonDifficulty;
	int _Objects;
	int _EditorTime;
	int _EditorTimeTotal;
	int _SongID;

	float _normalPercent;
	float _practicePercent;
	
	bool _Auto;
	bool _Demon;
	bool _VerifiedCoins;
	bool _LDM;
	bool _Gauntlet;
	bool _2P;

	std::string _LevelName;
	std::string _Description;
	std::string _LevelString;
	std::string _XORPassword;
	std::string _UploadDate;
	std::string _UpdateDate;
	std::string _ExtraString;
	std::string _Settings;
	std::string _RecordString;
	std::string _SondURL;
	std::string _LevelCreator;

	// Expects RobTop like string
	static GJGameLevel *createWithResponse(std::string backendResponse);
	GJGameLevel(){}
	GJGameLevel(std::string levelName, int levelID);
	static GJGameLevel *createWithMinimumData(std::string levelName, std::string creatorNickname, int levelID);
	static GJGameLevel *create();

	static std::string getLevelStrFromID(int gdLevelID);
	static std::string decompressLvlStr(std::string compressedLvlStr);
};
