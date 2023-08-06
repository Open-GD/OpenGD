/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#pragma once
#include <string>
#include <string_view>

class GJGameLevel;

enum DifficultyType {
	kMainLevels = 0,
	kLevelCell,
	kLevelInfoLayer
};

class GJGameLevel { 
public:

	//initialize everything otherwise some stuff just gets set to strange values idk

	int _levelID = 0;
	int _version = 0;
	int _playerID = 0;
	int _downloads = 0;
	int _musicID = 0;
	int _likes = 0;
	int _length = 0;
	int _difficultyDenominator = 0;
	int _difficultyNumerator = 0;
	int _setCompletes = 0;
	int _gameVersion = 0;
	int _dislikes = 0;
	int _stars = 0;
	int _featureScore = 0;
	int _copiedID = 0;
	int _coins = 0;
	int _starsRequested = 0;
	int _dailyNumber = 0;
	int _epic = 0;
	int _demonDifficulty = 0;
	int _objects = 0;
	int _editorTime = 0;
	int _editorTimeTotal = 0;
	int _songID = 0;
	int _officialSongID = 0;

	float _normalPercent = 0.f;
	float _practicePercent = 0.f;
	
	bool _auto = false;
	bool _demon = false;
	bool _verifiedCoins = false;
	bool _LDM = false;
	bool _gauntlet = false;
	bool _2P = false;

	std::string _levelName{};
	std::string _description{};
	std::string _levelString{};
	std::string _XORPassword{};
	std::string _uploadDate{};
	std::string _updateDate{};
	std::string _extraString{};
	std::string _settings{};
	std::string _recordString{};
	std::string _sondURL{};
	std::string _songName{};
	std::string _levelCreator{};

	// Expects RobTop like string. If it not, nullptr gets returned.
	static GJGameLevel *createWithResponse(std::string_view backendResponse);
	static GJGameLevel *createWithMinimumData(std::string levelName, std::string creatorNickname, int levelID);
	static GJGameLevel *create();

	static std::string getLevelStrFromID(int gdLevelID);
	static std::string decompressLvlStr(std::string compressedLvlStr);
	static std::string compressLvlStr(std::string decompressedLvlStr, int gdLevelID);

	static std::string getDifficultySprite(GJGameLevel* level, DifficultyType type = kLevelCell);
};
